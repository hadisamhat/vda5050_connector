#include "vda5050_connector/ManagerFSM.hpp"

using namespace vda5050_connector::interface;
using namespace Aws::Crt;
using namespace std::chrono;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

template <>
const std::map<FSMState, std::string> ManagerFSM<>::state_to_str_{{FSMState::INIT, "INIT"},
    {FSMState::OPERATIONAL, "OPERATIONAL"}, {FSMState::CONNECTED, "CONNECTED"},
    {FSMState::DISCONNECTED, "DISCONNECTED"}, {FSMState::ERROR, "ERROR"}};

template <>
void ManagerFSM<>::initializeStates() {
  state_machine_->addState(
      FSMState::INIT,
      [this] {
        auto connection = initializeTLS();
        if (!connection) {
          logger_->logError("Failed to Initialize TLS connection object");
          error_ = true;
          return;
        }
        connection_ = connection.value();
        connection_->SetReconnectTimeout(
            config_.min_reconnect_backoff_sec, config_.max_reconnect_backoff_sec);

        loadTopicInfoFromConfig(SupportedTopic::CONNECTION);
        loadTopicInfoFromConfig(SupportedTopic::STATE);
        loadTopicInfoFromConfig(SupportedTopic::ORDER);
        loadTopicInfoFromConfig(SupportedTopic::INSTANT_ACTION);
        loadTopicInfoFromConfig(SupportedTopic::VISUALIZATION);
        loadTopicInfoFromConfig(SupportedTopic::FACT_SHEET);
        tls_initialized_ = true;
      },
      [this] {}, [this] {});
  state_machine_->addState(
      FSMState::CONNECTED,
      [this] {
        // Invoked when a MQTT connect has completed or failed
        auto onConnectionCompleted = [&](Aws::Crt::Mqtt::MqttConnection&, int errorCode,
                                         Aws::Crt::Mqtt::ReturnCode returnCode, bool) {
          if (errorCode) {
            logger_->logError(
                "Connection failed with error \n" + std::string(ErrorDebugString(errorCode)));
            connection_completed_promise_.set_value(false);
            error_ = true;
          } else {
            logger_->logInfo(
                "Connection completed with return code \n" + std::to_string(returnCode));
            connection_completed_promise_.set_value(true);
          }
        };

        // Invoked when a MQTT connection was interrupted/lost
        auto onInterrupted = [this](Aws::Crt::Mqtt::MqttConnection&, int error) {
          logger_->logError(
              "Connection interrupted with error \n" + std::string(ErrorDebugString(error)));
        };

        // Invoked when a MQTT connection was interrupted/lost, but then reconnected successfully
        auto onResumed = [this](Aws::Crt::Mqtt::MqttConnection&, Aws::Crt::Mqtt::ReturnCode, bool) {
          logger_->logInfo("Connection resumed\n");
        };

        // Invoked when a disconnect message has completed.
        auto onDisconnect = [this](Aws::Crt::Mqtt::MqttConnection&) {
          logger_->logInfo("Disconnect completed\n");
          connection_closed_promise_.set_value();
        };

        connection_->OnConnectionCompleted = std::move(onConnectionCompleted);
        connection_->OnDisconnect = std::move(onDisconnect);
        connection_->OnConnectionInterrupted = std::move(onInterrupted);
        connection_->OnConnectionResumed = std::move(onResumed);

        logger_->logInfo("Connecting to AWS broker ...");
        if (!connection_->Connect(client_id_.c_str(), false, 1000)) {
          logger_->logError("Failed to Connect To AWS Broker " +
                            std::string(ErrorDebugString(connection_->LastError())));
          error_ = true;
        };

        // This is invoked upon the receipt of a Publish on a subscribed topic.
        auto onOrderReceived = [&](Mqtt::MqttConnection&, const String& topic,
                                   const ByteBuf& byteBuf, bool /*dup*/, Mqtt::QOS /*qos*/,
                                   bool /*retain*/) {
          std::string msg((char*)byteBuf.buffer);
          logger_->logInfo("Received message" + msg + "on topic " + topic.c_str());
          if (!on_order_received_) return;
          Json j;
          try {
            j = Json::parse(msg);
          } catch (std::exception& e) {
            error_ = true;
            logger_->logError("Error while parsing state message, " + std::string(e.what()));
            return;
          }
          rx_order.msg.from_json(j);
          on_order_received_(rx_order.msg);
        };

        // This is invoked upon the receipt of a Publish on a subscribed topic.
        auto onInstantActionReceived = [&](Mqtt::MqttConnection&, const String& topic,
                                           const ByteBuf& byteBuf, bool /*dup*/, Mqtt::QOS /*qos*/,
                                           bool /*retain*/) {
          std::string msg((char*)byteBuf.buffer);
          logger_->logInfo("Received message " + msg + " on topic " + std::string(topic));
          if (!on_instant_action_received_) return;
          Json j;
          try {
            j = Json::parse(msg);
          } catch (std::exception& e) {
            error_ = true;
            logger_->logError("Error while parsing state message, " + std::string(e.what()));
            return;
          }
          rx_instant_action.msg.from_json(j);
          on_instant_action_received_(rx_instant_action.msg);
        };
        // Subscribe for incoming publish messages on topic.
        auto onSubscriberAck = [&](Mqtt::MqttConnection&, uint16_t packetId, const String& topic,
                                   Mqtt::QOS QoS, int errorCode) {
          if (errorCode) {
            logger_->logError("Subscribe on topic " + std::string(topic) + "failed with error \n" +
                              std::string(aws_error_debug_str(errorCode)));
            error_ = true;
            return;
          }
          if (!packetId || QoS == AWS_MQTT_QOS_FAILURE) {
            logger_->logError("Subscribe rejected by the broker on topic." + std::string(topic));
            error_ = true;
            return;
          }
          logger_->logInfo("Subscribe on topic " + std::string(topic) + " Succeeded\n");
        };
        /// connect to subscribers
        connection_->Subscribe(rx_order.topic_name.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE,
            onOrderReceived, onSubscriberAck);
        connection_->Subscribe(rx_instant_action.topic_name.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE,
            onInstantActionReceived, onSubscriberAck);
      },
      [this] {}, [this] {});
  state_machine_->addState(
      FSMState::OPERATIONAL,
      [this] {
        if (tx_state.enable) {
          setupPublisher(state_timer_, tx_state.topic_name, tx_state.update_time_s, [this]() {
            tx_state.msg.header.headerId++;
            tx_state.msg.header.timestamp = getISOCurrentTimestamp();
            tx_state.msg.header.version = config_.protocol_version;
            tx_state.msg.header.manufacturer = config_.manufacturer;
            tx_state.msg.header.serialNumber = config_.serial_number;
            auto j = tx_state.msg.to_json();
            // logger_->logInfo("state timeout elapsed, publishing data" + j.dump());
            //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
            //   j.dump().length()); connection_->Publish(tx_visualization.topic_name.c_str(),
            //   AWS_MQTT_QOS_AT_LEAST_ONCE,
            //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
          });
        }
        if (tx_connection.enable) {
          setupPublisher(
              connection_timer_, tx_connection.topic_name, tx_connection.update_time_s, [this]() {
                tx_connection.msg.header.headerId++;
                tx_connection.msg.header.timestamp = getISOCurrentTimestamp();
                tx_connection.msg.header.version = config_.protocol_version;
                tx_connection.msg.header.manufacturer = config_.manufacturer;
                tx_connection.msg.header.serialNumber = config_.serial_number;
                auto j = tx_connection.msg.to_json();
                // logger_->logInfo("Connection timeout elapsed, publishing data" + j.dump());
                //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
                //   j.dump().length()); connection_->Publish(tx_visualization.topic_name.c_str(),
                //   AWS_MQTT_QOS_AT_LEAST_ONCE,
                //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
              });
        }
        if (tx_fact_sheet.enable) {
          setupPublisher(
              fact_sheet_timer_, tx_fact_sheet.topic_name, tx_fact_sheet.update_time_s, [this]() {
                tx_fact_sheet.msg.header.headerId++;
                tx_fact_sheet.msg.header.timestamp = getISOCurrentTimestamp();
                tx_fact_sheet.msg.header.version = config_.protocol_version;
                tx_fact_sheet.msg.header.manufacturer = config_.manufacturer;
                tx_fact_sheet.msg.header.serialNumber = config_.serial_number;
                auto j = tx_fact_sheet.msg.to_json();
                // logger_->logInfo("Fact Sheet timeout elapsed, publishing data" + j.dump());
                //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
                //   j.dump().length()); connection_->Publish(tx_visualization.topic_name.c_str(),
                //   AWS_MQTT_QOS_AT_LEAST_ONCE,
                //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
              });
        }
        if (tx_visualization.enable) {
          setupPublisher(visualization_timer_, tx_visualization.topic_name,
              tx_visualization.update_time_s, [this]() {
                tx_visualization.msg.header.headerId++;
                tx_visualization.msg.header.timestamp = getISOCurrentTimestamp();
                tx_visualization.msg.header.version = config_.protocol_version;
                tx_visualization.msg.header.manufacturer = config_.manufacturer;
                tx_visualization.msg.header.serialNumber = config_.serial_number;
                auto j = tx_visualization.msg.to_json();
                // logger_->logInfo("Visualization timeout elapsed, publishing data" + j.dump());
                //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
                //   j.dump().length()); connection_->Publish(tx_visualization.topic_name.c_str(),
                //   AWS_MQTT_QOS_AT_LEAST_ONCE,
                //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
              });
        }
        if (p_callback_) {
          p_callback_();
        }
      },
      [this] {}, [this] {});
  state_machine_->addState(
      FSMState::DISCONNECTED,
      [this] { logger_->logInfo("AWS client Failed to connect. Retrying"); }, [this] {}, [this] {});
  state_machine_->addState(
      FSMState::ERROR,
      [this] { logger_->logInfo("An error has occurred. Check logs for more information."); },
      [this] {}, [this] {});
}
}  // namespace impl
}  // namespace vda5050_connector