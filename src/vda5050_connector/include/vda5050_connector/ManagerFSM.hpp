#pragma once
#include <aws/crt/Api.h>
#include <aws/crt/StlAllocator.h>
#include <aws/crt/UUID.h>
#include <aws/crt/auth/Credentials.h>
#include <aws/crt/io/TlsOptions.h>
#include <aws/iot/MqttClient.h>

#include <algorithm>
#include <boost/date_time.hpp>
#include <chrono>
#include <condition_variable>
#include <cstdarg>
#include <cstring>
#include <experimental/filesystem>
#include <experimental/optional>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

#include "iw_logging/std_logger.hpp"
#include "iw_state_machine/state_machine.hpp"
#include "vda5050_connector_interface/BaseManagerInterface.hpp"

using namespace vda5050_connector::interface;
using namespace Aws::Crt;
using namespace std::chrono;
using namespace std;
using Json = nlohmann::json;
namespace filesys = std::experimental::filesystem;
namespace vda5050_connector {
namespace impl {

enum class SupportedTopic { STATE, ORDER, INSTANT_ACTION, CONNECTION, VISUALIZATION, FACT_SHEET };

template <class OrderMsg, class InstantActionMsg, class StateMsg, class VisualizationMsg,
    class ConnectionMsg, class FactSheetMsg>
class ManagerFSM : public interface::BaseManagerInterface<OrderMsg, InstantActionMsg, StateMsg,
                       VisualizationMsg, ConnectionMsg, FactSheetMsg> {
 public:
  ManagerFSM(interface::BaseNetworkConfiguration config, boost::asio::io_context& io_context)
      : logger_(std::make_shared<iw::logging::StdLogger>()),
        state_machine_(std::make_shared<
            iw::state_machine::StateMachine<vda5050_connector::interface::FSMState>>(
            getName(), logger_)),
        io_context_(io_context),
        tick_timer_(io_context_),
        state_timer_(io_context_),
        visualization_timer_(io_context_),
        connection_timer_(io_context_),
        fact_sheet_timer_(io_context_) {
    config_ = config;
  }

  void createStateMachine() override {
    state_machine_->setToString(this->getStateName);
    initializeStates();
    initializeTransitions();
  }

  void start() override {
    createStateMachine();
    state_machine_->start(FSMState::INIT);
    tick();
  }

  void tick() override {
    tick_timer_.expires_after(std::chrono::milliseconds(100));
    tick_timer_.async_wait([this](const boost::system::error_code& ec) {
      if (stop_.load()) return;
      if (ec == boost::system::errc::operation_canceled) {
        logger_->logError("tick is cancelled: " + std::string(ec.message()));
        return;
      }
      state_machine_->tick();
      tick();
    });
  }

  void stop() override {
    disconnect();
    stop_.store(true);
    io_context_.post([&]() {
      tick_timer_.cancel();
      state_timer_.cancel();
      visualization_timer_.cancel();
      connection_timer_.cancel();
      fact_sheet_timer_.cancel();
    });
  }

  std::string getName() const override { return "AWSManagerFSM"; }

  std::string getCurrentStateName() override { return state_machine_->getCurrentStateName(); }
  vda5050_connector::interface::FSMState getCurrentState() override {
    if (state_machine_->current_state())
      return state_machine_->current_state().value();
    else
      return vda5050_connector::interface::FSMState::ERROR;
  }

  static std::string getStateName(const vda5050_connector::interface::FSMState& state) {
    try {
      return state_to_str_.at(state);
    } catch (std::out_of_range& exception) {
      return std::string{"UNDEF_STATE_NAME"};
    }
  }

  void setupPublisher(boost::asio::steady_timer& timer, const std::string& topic,
      const double& update_time, const std::function<void()>& func) override {
    std::chrono::duration<double> duration(update_time);
    auto time_s = std::chrono::duration_cast<std::chrono::seconds>(duration);
    timer.expires_after(std::chrono::seconds(time_s));
    timer.async_wait([&timer, topic, update_time, func, this](const boost::system::error_code& ec) {
      if (stop_.load()) return;

      if (ec == boost::system::errc::operation_canceled) {
        logger_->logError("publisher timer is cancelled: " + std::string(ec.message()));
        return;
      }
      func();
      setupPublisher(timer, topic, update_time, func);
    });
  }

  void setPublishers(std::function<void()>& func) { p_callback_ = func; }

  OrderMsg getOrderMsg() override {
    std::lock_guard<std::mutex> lock(rx_order_.sub_mutex);
    return rx_order_.msg;
  }

  InstantActionMsg getInstantActionMsg() {
    std::lock_guard<std::mutex> lock(rx_instant_action_.sub_mutex);
    return rx_instant_action_.msg;
  };

  void updateStateMsg(const std::function<void(StateMsg&)>& func) override {
    func(tx_state_.msg);
    io_context_.post([this]() {
      tx_state_.msg.header.headerId++;
      tx_state_.msg.header.timestamp = getISOCurrentTimestamp();
      tx_state_.msg.header.version = config_.protocol_version;
      tx_state_.msg.header.manufacturer = config_.manufacturer;
      tx_state_.msg.header.serialNumber = config_.serial_number;
      auto j = tx_state_.msg.to_json();
      logger_->logInfo("publishing update state msg" + j.dump());
      //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
      //   j.dump().length()); connection_->Publish(tx_visualization_.topic_name.c_str(),
      //   AWS_MQTT_QOS_AT_LEAST_ONCE,
      //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
    });
  }

  void updateVisualizationMsg(const std::function<void(VisualizationMsg&)>& func) override {
    func(tx_visualization_.msg);
    io_context_.post([this]() {
      tx_visualization_.msg.header.headerId++;
      tx_visualization_.msg.header.timestamp = getISOCurrentTimestamp();
      tx_visualization_.msg.header.version = config_.protocol_version;
      tx_visualization_.msg.header.manufacturer = config_.manufacturer;
      tx_visualization_.msg.header.serialNumber = config_.serial_number;
      auto j = tx_visualization_.msg.to_json();
      logger_->logInfo("publishing update visualization msg" + j.dump());
      //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
      //   j.dump().length()); connection_->Publish(tx_visualization_.topic_name.c_str(),
      //   AWS_MQTT_QOS_AT_LEAST_ONCE,
      //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
    });
  }

  void updateConnectionMsg(const std::function<void(ConnectionMsg&)>& func) {
    func(tx_connection_.msg);
    io_context_.post([this]() {
      tx_connection_.msg.header.headerId++;
      tx_connection_.msg.header.timestamp = getISOCurrentTimestamp();
      tx_connection_.msg.header.version = config_.protocol_version;
      tx_connection_.msg.header.manufacturer = config_.manufacturer;
      tx_connection_.msg.header.serialNumber = config_.serial_number;
      auto j = tx_connection_.msg.to_json();
      logger_->logInfo("publishing update connection msg" + j.dump());
      //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
      //   j.dump().length()); connection_->Publish(tx_visualization_.topic_name.c_str(),
      //   AWS_MQTT_QOS_AT_LEAST_ONCE,
      //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
    });
  }

  void updateFactSheetMsg(const std::function<void(FactSheetMsg&)>& func) override {
    func(tx_fact_sheet_.msg);
    io_context_.post([this]() {
      tx_fact_sheet_.msg.header.headerId++;
      tx_fact_sheet_.msg.header.timestamp = getISOCurrentTimestamp();
      tx_fact_sheet_.msg.header.version = config_.protocol_version;
      tx_fact_sheet_.msg.header.manufacturer = config_.manufacturer;
      tx_fact_sheet_.msg.header.serialNumber = config_.serial_number;
      auto j = tx_fact_sheet_.msg.to_json();
      logger_->logInfo("publishing update fact_sheet msg" + j.dump());
      //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
      //   j.dump().length()); connection_->Publish(tx_visualization_.topic_name.c_str(),
      //   AWS_MQTT_QOS_AT_LEAST_ONCE,
      //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
    });
  }

  void registerSubscriber(std::string topic_name, const std::function<void(Json&)>& onReceive) {
    if (error_) return;
    auto onMsgReceived = [onReceive, this](Mqtt::MqttConnection&, const String& topic,
                             const ByteBuf& byteBuf, bool /*dup*/, Mqtt::QOS /*qos*/,
                             bool /*retain*/) {
      std::string msg((char*)byteBuf.buffer, byteBuf.len);
      logger_->logInfo("Received message " + msg + " on topic " + topic.c_str());
      Json j;
      try {
        j = Json::parse(msg);
      } catch (std::exception& e) {
        error_ = "Error while parsing message, " + std::string(e.what());
        return;
      }
      onReceive(j);
    };
    auto onSubscriberAck = [&](Mqtt::MqttConnection&, uint16_t packetId, const String& topic,
                               Mqtt::QOS QoS, int errorCode) {
      if (errorCode) {
        error_ = "Subscribe on topic " + std::string(topic.c_str()) + "failed with error \n" +
                 std::string(aws_error_debug_str(errorCode));
        return;
      }
      if (!packetId || QoS == AWS_MQTT_QOS_FAILURE) {
        error_ = "Subscribe rejected by the broker on topic." + std::string(topic.c_str());
        return;
      }
      logger_->logInfo("Subscribe on topic" + std::string(topic.c_str()) + " Succeeded\n");
    };
    connection_->Subscribe(
        topic_name.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE, onMsgReceived, onSubscriberAck);
  }

  void setOnOrderReceived(const std::function<void(OrderMsg&)>& func) { on_order_received_ = func; }

  void setOnInstantActionReceived(const std::function<void(InstantActionMsg&)>& func) {
    on_instant_action_received_ = func;
  };

 protected:
  Aws::Crt::ApiHandle apiHandle;
  std::experimental::optional<std::string> error_;
  std::atomic<bool> stop_{false};

  std::string client_id_;
  interface::BaseNetworkConfiguration config_;
  BaseSubscribedTopic<OrderMsg> rx_order_;
  BaseSubscribedTopic<InstantActionMsg> rx_instant_action_;
  BasePublishedTopic<StateMsg> tx_state_;
  BasePublishedTopic<VisualizationMsg> tx_visualization_;
  BasePublishedTopic<ConnectionMsg> tx_connection_;
  BasePublishedTopic<FactSheetMsg> tx_fact_sheet_;

  std::function<void()> p_callback_;
  std::function<void(OrderMsg&)> on_order_received_;
  std::function<void(InstantActionMsg&)> on_instant_action_received_;

  boost::asio::io_context& io_context_;
  boost::asio::steady_timer state_timer_;
  boost::asio::steady_timer visualization_timer_;
  boost::asio::steady_timer connection_timer_;
  boost::asio::steady_timer fact_sheet_timer_;
  boost::asio::steady_timer tick_timer_;

  std::shared_ptr<Aws::Crt::Mqtt::MqttConnection> connection_;
  std::shared_ptr<Aws::Iot::MqttClient> client_;
  std::shared_ptr<iw::state_machine::StateMachine<vda5050_connector::interface::FSMState>>
      state_machine_;
  /// Message logger
  std::shared_ptr<iw::logging::StdLogger> logger_;
  std::promise<bool> connection_completed_promise_;
  std::promise<void> connection_closed_promise_;

  static const std::map<vda5050_connector::interface::FSMState, std::string> state_to_str_;

  std::string getISOCurrentTimestamp() {
    boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
    auto timestamp = boost::posix_time::to_iso_extended_string(t);
    // Remove last three  numbers from the iso string to only contain three numbers after the last
    // dot.
    timestamp.pop_back();
    timestamp.pop_back();
    timestamp.pop_back();
    timestamp.append("Z");
    return timestamp;
  }

  void initializeStates() override {
    state_machine_->addState(
        FSMState::INIT,
        [this] {
          auto connection = initializeTLS();
          if (!connection) {
            error_ = "Failed to Initialize TLS connection object";
            return;
          }
          connection_ = connection.value();
          connection_->SetReconnectTimeout(
              config_.min_reconnect_backoff_sec, config_.max_reconnect_backoff_sec);

          loadTopicInfoFromConfig();

          //  Invoked when a MQTT connect has completed or failed
          connection_->OnConnectionCompleted = [&](Aws::Crt::Mqtt::MqttConnection&, int errorCode,
                                                   Aws::Crt::Mqtt::ReturnCode returnCode, bool) {
            if (errorCode) {
              connection_completed_promise_.set_value(false);
              error_ = "Connection failed with error" + std::string(ErrorDebugString(errorCode));
            } else {
              logger_->logInfo(
                  "Connection completed with return code " + std::to_string(returnCode));
              connection_completed_promise_.set_value(true);
            }
          };

          connection_->OnDisconnect = [this](Aws::Crt::Mqtt::MqttConnection&) {
            logger_->logInfo("Disconnect completed");
            connection_closed_promise_.set_value();
          };

          // Invoked when a MQTT connection was interrupted/lost
          connection_->OnConnectionInterrupted = [this](
                                                     Aws::Crt::Mqtt::MqttConnection&, int error) {
            logger_->logError(
                "Connection interrupted with error" + std::string(ErrorDebugString(error)));
          };
          // Invoked when a MQTT connection was interrupted/lost, but then reconnected
          // successfully
          connection_->OnConnectionResumed = [this](Aws::Crt::Mqtt::MqttConnection&,
                                                 Aws::Crt::Mqtt::ReturnCode,
                                                 bool) { logger_->logInfo("Connection resumed"); };

          logger_->logInfo("Connecting to AWS broker ...");
          if (!connection_->Connect(client_id_.c_str(), false, 1000)) {
            error_ = "Failed to Connect To AWS Broker " +
                     std::string(ErrorDebugString(connection_->LastError()));
          };

          // This is invoked upon the receipt of a Publish on a subscribed topic.
          auto onOrderReceived = [&](Mqtt::MqttConnection&, const String& topic,
                                     const ByteBuf& byteBuf, bool /*dup*/, Mqtt::QOS /*qos*/,
                                     bool /*retain*/) {
            std::string msg((char*)byteBuf.buffer, byteBuf.len);
            logger_->logInfo("Received message " + msg + "on topic " + topic.c_str());
            if (!on_order_received_) return;
            Json j;
            try {
              j = Json::parse(msg);
            } catch (std::exception& e) {
              error_ = "Error while parsing state message, " + std::string(e.what());
              return;
            }
            std::lock_guard<std::mutex> lock(rx_order_.sub_mutex);
            rx_order_.msg.from_json(j);
            on_order_received_(rx_order_.msg);
          };

          // This is invoked upon the receipt of a Publish on a subscribed topic.
          auto onInstantActionReceived = [&](Mqtt::MqttConnection&, const String& topic,
                                             const ByteBuf& byteBuf, bool /*dup*/,
                                             Mqtt::QOS /*qos*/, bool /*retain*/) {
            std::string msg((char*)byteBuf.buffer, byteBuf.len);
            logger_->logInfo("Received message " + msg + " on topic " + std::string(topic.c_str()));
            if (!on_instant_action_received_) return;
            Json j;
            try {
              j = Json::parse(msg);
            } catch (std::exception& e) {
              error_ = "Error while parsing state message, " + std::string(e.what());
              return;
            }
            std::lock_guard<std::mutex> lock(rx_instant_action_.sub_mutex);
            rx_instant_action_.msg.from_json(j);
            on_instant_action_received_(rx_instant_action_.msg);
          };
          // Subscribe for incoming publish messages on topic.
          auto onSubscriberAck = [&](Mqtt::MqttConnection&, uint16_t packetId, const String& topic,
                                     Mqtt::QOS QoS, int errorCode) {
            if (errorCode) {
              error_ = "Subscribe on topic " + std::string(topic.c_str()) + "failed with error \n" +
                       std::string(aws_error_debug_str(errorCode));
              return;
            }
            if (!packetId || QoS == AWS_MQTT_QOS_FAILURE) {
              error_ = "Subscribe rejected by the broker on topic." + std::string(topic.c_str());
              return;
            }
            logger_->logInfo("Subscribe on topic " + std::string(topic.c_str()) + " Succeeded\n");
          };
          /// connect to subscribers
          connection_->Subscribe(rx_order_.topic_name.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE,
              onOrderReceived, onSubscriberAck);
          connection_->Subscribe(rx_instant_action_.topic_name.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE,
              onInstantActionReceived, onSubscriberAck);
        },
        [this] {}, [this] {});
    state_machine_->addState(
        FSMState::OPERATIONAL,
        [this] {
          if (tx_state_.enable) {
            setupPublisher(state_timer_, tx_state_.topic_name, tx_state_.update_time_s, [this]() {
              tx_state_.msg.header.headerId++;
              tx_state_.msg.header.timestamp = getISOCurrentTimestamp();
              tx_state_.msg.header.version = config_.protocol_version;
              tx_state_.msg.header.manufacturer = config_.manufacturer;
              tx_state_.msg.header.serialNumber = config_.serial_number;
              auto j = tx_state_.msg.to_json();
              logger_->logInfo("state timeout elapsed, publishing data" + j.dump());
              //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
              //   j.dump().length()); connection_->Publish(tx_visualization_.topic_name.c_str(),
              //   AWS_MQTT_QOS_AT_LEAST_ONCE,
              //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
            });
          }
          if (tx_connection_.enable) {
            setupPublisher(connection_timer_, tx_connection_.topic_name,
                tx_connection_.update_time_s, [this]() {
                  tx_connection_.msg.header.headerId++;
                  tx_connection_.msg.header.timestamp = getISOCurrentTimestamp();
                  tx_connection_.msg.header.version = config_.protocol_version;
                  tx_connection_.msg.header.manufacturer = config_.manufacturer;
                  tx_connection_.msg.header.serialNumber = config_.serial_number;
                  auto j = tx_connection_.msg.to_json();
                  logger_->logInfo("Connection timeout elapsed, publishing data" + j.dump());
                  //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
                  //   j.dump().length());
                  //   connection_->Publish(tx_visualization_.topic_name.c_str(),
                  //   AWS_MQTT_QOS_AT_LEAST_ONCE,
                  //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
                });
          }
          if (tx_fact_sheet_.enable) {
            setupPublisher(fact_sheet_timer_, tx_fact_sheet_.topic_name,
                tx_fact_sheet_.update_time_s, [this]() {
                  tx_fact_sheet_.msg.header.headerId++;
                  tx_fact_sheet_.msg.header.timestamp = getISOCurrentTimestamp();
                  tx_fact_sheet_.msg.header.version = config_.protocol_version;
                  tx_fact_sheet_.msg.header.manufacturer = config_.manufacturer;
                  tx_fact_sheet_.msg.header.serialNumber = config_.serial_number;
                  auto j = tx_fact_sheet_.msg.to_json();
                  logger_->logInfo("Fact Sheet timeout elapsed, publishing data" + j.dump());
                  //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
                  //   j.dump().length());
                  //   connection_->Publish(tx_visualization_.topic_name.c_str(),
                  //   AWS_MQTT_QOS_AT_LEAST_ONCE,
                  //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
                });
          }
          if (tx_visualization_.enable) {
            setupPublisher(visualization_timer_, tx_visualization_.topic_name,
                tx_visualization_.update_time_s, [this]() {
                  tx_visualization_.msg.header.headerId++;
                  tx_visualization_.msg.header.timestamp = getISOCurrentTimestamp();
                  tx_visualization_.msg.header.version = config_.protocol_version;
                  tx_visualization_.msg.header.manufacturer = config_.manufacturer;
                  tx_visualization_.msg.header.serialNumber = config_.serial_number;
                  auto j = tx_visualization_.msg.to_json();
                  logger_->logInfo("Visualization timeout elapsed, publishing data" + j.dump());
                  //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
                  //   j.dump().length());
                  //   connection_->Publish(tx_visualization_.topic_name.c_str(),
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
        FSMState::ERROR,
        [this] {
          logger_->logInfo("An error has occurred " + error_.value());
          throw vda5050::exception::Vda5050Exception(error_.value().c_str());
        },
        [this] {}, [this] {});
  }

  void initializeTransitions() override {
    state_machine_->addTransition(
        FSMState::INIT, FSMState::OPERATIONAL,
        [this] { return !error_ && connection_completed_promise_.get_future().get(); }, [this] {});
    for (const auto& state_pair : state_to_str_) {
      const auto state = state_pair.first;
      if (state != FSMState::ERROR) {
        state_machine_->addTransition(
            state, FSMState::ERROR, [this] { return false; }, [this] {});
      }
    }
  }

  void disconnect() {
    if (error_) return;
    // Unsubscribe from the topic.
    std::promise<void> unsubscribe_instant_action_finished_promise;
    std::promise<void> unsubscribe_order_finished_promise;

    connection_->Unsubscribe(
        rx_instant_action_.topic_name.c_str(), [&](Mqtt::MqttConnection&, uint16_t, int) {
          unsubscribe_instant_action_finished_promise.set_value();
        });
    unsubscribe_instant_action_finished_promise.get_future().wait();

    connection_->Unsubscribe(
        rx_order_.topic_name.c_str(), [&](Mqtt::MqttConnection&, uint16_t, int) {
          unsubscribe_order_finished_promise.set_value();
        });
    unsubscribe_order_finished_promise.get_future().wait();
    // Disconnect

    connection_->Disconnect();
    connection_closed_promise_.get_future().wait();
  }

  std::experimental::optional<std::shared_ptr<Aws::Crt::Mqtt::MqttConnection>> initializeTLS() {
    if (!filesExist({config_.root_ca_path, config_.cert_path, config_.priv_key_path,
            config_.client_id_path})) {
      logger_->logError("Required files do not exist.");
      return std::experimental::nullopt;
    }

    // Fill the client Id value, by reading it from the id file.
    std::ifstream clientIdFile;
    // TODO: Should be configured by the onboarder or a configurator.
    clientIdFile.open(config_.client_id_path);
    getline(clientIdFile, client_id_);
    clientIdFile.close();

    auto clientConfigBuilder = Aws::Iot::MqttClientConnectionConfigBuilder(
        config_.cert_path.c_str(), config_.priv_key_path.c_str());
    clientConfigBuilder.WithEndpoint(config_.endpoint.c_str());

    clientConfigBuilder.WithCertificateAuthority(config_.root_ca_path.c_str());

    if (config_.mqtt_port != 0) {
      clientConfigBuilder.WithPortOverride(static_cast<uint16_t>(config_.mqtt_port));
    }
    // Create the MQTT connection from the MQTT builder
    auto clientConfig = clientConfigBuilder.Build();
    if (!clientConfig) {
      logger_->logError("Client Configuration initialization failed with error \n" +
                        std::string(ErrorDebugString(clientConfig.LastError())));
      return std::experimental::nullopt;
    }
    client_ = std::make_shared<Aws::Iot::MqttClient>();
    auto connection = client_->NewConnection(clientConfig);
    if (!*connection) {
      logger_->logError("MQTT Connection Creation failed with error \n" +
                        std::string(ErrorDebugString(connection->LastError())));
      return std::experimental::nullopt;
    }
    return connection;
  }

  bool filesExist(std::vector<std::string> filePaths) {
    for (const auto& path : filePaths) {
      // Check if the specified file exists.
      if (!filesys::exists(path)) {
        return false;
      }
    }
    return true;
  }

  void loadTopicInfoFromConfig() {
    std::string prefix =
        ((config_.mode == "dev") ? config_.dev_topic_prefix : config_.qa_topic_prefix + "/")
            .append(client_id_)
            .append("/");

    rx_order_.topic_name = prefix + config_.order_topic_name;
    rx_instant_action_.topic_name = prefix + config_.instant_action_topic_name;

    tx_state_.topic_name = prefix + config_.state_topic_name;
    tx_state_.update_time_s = config_.state_interval_secs;

    tx_connection_.topic_name = prefix + config_.connection_topic_name;
    tx_connection_.update_time_s = config_.connection_state_interval_secs;

    tx_visualization_.topic_name = prefix + config_.visualization_topic_name;
    tx_visualization_.update_time_s = config_.visualization_interval_secs;

    tx_fact_sheet_.topic_name = prefix + config_.fact_sheet_topic_name;
    tx_fact_sheet_.update_time_s = config_.fact_sheet_interval_secs;
  }
};

template <class OrderMsg, class InstantActionMsg, class StateMsg, class VisualizationMsg,
    class ConnectionMsg, class FactSheetMsg>
const std::map<FSMState, std::string> ManagerFSM<OrderMsg, InstantActionMsg, StateMsg,
    VisualizationMsg, ConnectionMsg, FactSheetMsg>::state_to_str_{
    {FSMState::INIT, "INIT"}, {FSMState::OPERATIONAL, "OPERATIONAL"}, {FSMState::ERROR, "ERROR"}};

}  // namespace impl
}  // namespace vda5050_connector