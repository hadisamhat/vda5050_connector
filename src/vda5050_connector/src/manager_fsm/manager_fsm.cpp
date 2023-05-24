#include "vda5050_connector/ManagerFSM.hpp"

using namespace vda5050_connector::interface;
using namespace Aws::Crt;
using namespace std::chrono;
using namespace std::experimental;
using Json = nlohmann::json;
namespace filesys = std::filesystem;
using namespace boost::posix_time;

namespace vda5050_connector {
namespace impl {

template <>
ManagerFSM<>::ManagerFSM(BaseNetworkConfiguration config)
    : logger_(std::make_shared<iw::logging::StdLogger>()),
      state_machine_(
          std::make_shared<iw::state_machine::StateMachine<vda5050_connector::interface::FSMState>>(
              getName(), logger_)),
      io_context_(std::make_shared<boost::asio::io_context>()),
      tick_timer_(*io_context_),
      state_timer_(*io_context_),
      visualization_timer_(*io_context_),
      connection_timer_(*io_context_),
      fact_sheet_timer_(*io_context_) {
  config_ = config;
};

template <>
std::string ManagerFSM<>::getISOCurrentTimestamp() {
  ptime t = microsec_clock::universal_time();
  auto timestamp = to_iso_extended_string(t);
  // Remove last three  numbers from the iso string to only contain three numbers after the last
  // dot.
  timestamp.pop_back();
  timestamp.pop_back();
  timestamp.pop_back();
  timestamp.append("Z");
  return timestamp;
};
template <>
void ManagerFSM<>::registerSubscriber(std::string topic_name, const std::function<void()>& onReceive) {
  if (!connection_completed_promise_.get_future().get()) return;
  std::promise<void> subscribe_finished_promise;
  auto onMsgReceived = [onReceive](Mqtt::MqttConnection&, const String& topic,
                           const ByteBuf& byteBuf, bool /*dup*/, Mqtt::QOS /*qos*/,
                           bool /*retain*/) { onReceive(); };
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
    logger_->logInfo("Subscribe on topic" + std::string(topic) + " Succeeded\n");
    subscribe_finished_promise.set_value();
  };
  connection_->Subscribe(
      topic_name.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE, onMsgReceived, onSubscriberAck);
  subscribe_finished_promise.get_future().wait();
}
template <>
Order ManagerFSM<>::getOrderMsg() {
  std::lock_guard<std::mutex> lock(rx_order.sub_mutex);
  return rx_order.msg;
}

template <>
InstantAction ManagerFSM<>::getInstantActionMsg() {
  std::lock_guard<std::mutex> lock(rx_instant_action.sub_mutex);
  return rx_instant_action.msg;
}

template <>
void ManagerFSM<>::updateStateMsg(const std::function<void(State&)>& func) {
  func(tx_state.msg);
  io_context_->post([this]() {
    tx_state.msg.header.headerId++;
    tx_state.msg.header.timestamp = getISOCurrentTimestamp();
    tx_state.msg.header.version = config_.protocol_version;
    tx_state.msg.header.manufacturer = config_.manufacturer;
    tx_state.msg.header.serialNumber = config_.serial_number;
    auto j = tx_state.msg.to_json();
    logger_->logInfo("publishing update state msg" + j.dump());
    //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
    //   j.dump().length()); connection_->Publish(tx_visualization.topic_name.c_str(),
    //   AWS_MQTT_QOS_AT_LEAST_ONCE,
    //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
  });
}

template <>
void ManagerFSM<>::updateVisualizationMsg(const std::function<void(Visualization&)>& func) {
  func(tx_visualization.msg);
  io_context_->post([this]() {
    tx_visualization.msg.header.headerId++;
    tx_visualization.msg.header.timestamp = getISOCurrentTimestamp();
    tx_visualization.msg.header.version = config_.protocol_version;
    tx_visualization.msg.header.manufacturer = config_.manufacturer;
    tx_visualization.msg.header.serialNumber = config_.serial_number;
    auto j = tx_visualization.msg.to_json();
    logger_->logInfo("publishing update visualization msg" + j.dump());
    //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
    //   j.dump().length()); connection_->Publish(tx_visualization.topic_name.c_str(),
    //   AWS_MQTT_QOS_AT_LEAST_ONCE,
    //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
  });
}

template <>
void ManagerFSM<>::updateConnectionMsg(const std::function<void(Connection&)>& func) {
  func(tx_connection.msg);
  io_context_->post([this]() {
    tx_connection.msg.header.headerId++;
    tx_connection.msg.header.timestamp = getISOCurrentTimestamp();
    tx_connection.msg.header.version = config_.protocol_version;
    tx_connection.msg.header.manufacturer = config_.manufacturer;
    tx_connection.msg.header.serialNumber = config_.serial_number;
    auto j = tx_connection.msg.to_json();
    logger_->logInfo("publishing update connection msg" + j.dump());
    //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
    //   j.dump().length()); connection_->Publish(tx_visualization.topic_name.c_str(),
    //   AWS_MQTT_QOS_AT_LEAST_ONCE,
    //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
  });
}

template <>
void ManagerFSM<>::updateFactSheetMsg(const std::function<void(FactSheet&)>& func) {
  func(tx_fact_sheet.msg);
  io_context_->post([this]() {
    tx_fact_sheet.msg.header.headerId++;
    tx_fact_sheet.msg.header.timestamp = getISOCurrentTimestamp();
    tx_fact_sheet.msg.header.version = config_.protocol_version;
    tx_fact_sheet.msg.header.manufacturer = config_.manufacturer;
    tx_fact_sheet.msg.header.serialNumber = config_.serial_number;
    auto j = tx_fact_sheet.msg.to_json();
    logger_->logInfo("publishing update fact_sheet msg" + j.dump());
    //   ByteBuf payload = ByteBufFromArray((const uint8_t*)j.dump().data(),
    //   j.dump().length()); connection_->Publish(tx_visualization.topic_name.c_str(),
    //   AWS_MQTT_QOS_AT_LEAST_ONCE,
    //       false, payload,  [](Mqtt::MqttConnection&, uint16_t, int) {});
  });
}
template <>
void ManagerFSM<>::setupPublisher(boost::asio::steady_timer& timer, const std::string& topic,
    const double& update_time, const std::function<void()>& function) {
  if (!function) return;

  std::chrono::duration<double> duration(update_time);
  auto time_s = std::chrono::duration_cast<std::chrono::seconds>(duration);
  timer.expires_after(std::chrono::seconds(time_s));
  timer.async_wait(
      [&timer, topic, update_time, function, this](const boost::system::error_code& ec) {
        if (stop_.load()) return;
        if (ec) {
          logger_->logError("timer is cancelled: " + std::string(ec.message()));
          return;
        }
        function();
        setupPublisher(timer, topic, update_time, function);
      });
}

template <>
void ManagerFSM<>::loadTopicInfoFromConfig(SupportedTopic t) {
  std::string prefix;

  if (config_.mode == "dev") {
    prefix = config_.dev_topic_prefix + "/";
  } else if (config_.mode == "qa") {
    prefix = config_.qa_topic_prefix + "/";
  }
  switch (t) {
    case SupportedTopic::ORDER:
      rx_order.topic_name = prefix.append(client_id_).append("/").append(config_.order_topic_name);
      break;
    case SupportedTopic::INSTANT_ACTION:
      rx_instant_action.topic_name =
          prefix.append(client_id_).append("/").append(config_.instant_action_topic_name);
      break;

    case SupportedTopic::STATE:
      tx_state.topic_name = prefix.append(client_id_).append("/").append(config_.state_topic_name);
      tx_state.update_time_s = config_.state_interval_secs;
      break;

    case SupportedTopic::CONNECTION:
      tx_connection.topic_name =
          prefix.append(client_id_).append("/").append(config_.connection_topic_name);
      tx_connection.update_time_s = config_.connection_state_interval_secs;
      break;

    case SupportedTopic::VISUALIZATION:
      tx_visualization.topic_name =
          prefix.append(client_id_).append("/").append(config_.visualization_topic_name);
      tx_visualization.update_time_s = config_.visualization_interval_secs;
      break;

    case SupportedTopic::FACT_SHEET:
      tx_fact_sheet.topic_name =
          prefix.append(client_id_).append("/").append(config_.fact_sheet_topic_name);
      tx_fact_sheet.update_time_s = config_.fact_sheet_interval_secs;
      break;

    default:
      return;
  }
}

template <>
void ManagerFSM<>::createStateMachine() {
  state_machine_->setToString(this->getStateName);
  initializeStates();
  initializeTransitions();
}

template <>
void ManagerFSM<>::tick() {
  tick_timer_.expires_after(std::chrono::milliseconds(100));
  tick_timer_.async_wait([this](const boost::system::error_code& ec) {
    if (stop_.load()) return;
    if (ec) {
      logger_->logError("timer is cancelled: " + std::string(ec.message()));
      return;
    }
    state_machine_->tick();
    tick();
  });
}

template <>
void ManagerFSM<>::start() {
  createStateMachine();
  state_machine_->start(FSMState::INIT);
  tick();
  worker_thread_ = std::thread([this]() { io_context_->run(); });
}

template <>
void ManagerFSM<>::disconnect() {
  // Unsubscribe from the topic.
  std::promise<void> unsubscribe_instant_action_finished_promise;
  std::promise<void> unsubscribe_order_finished_promise;

  connection_->Unsubscribe(
      rx_instant_action.topic_name.c_str(), [&](Mqtt::MqttConnection&, uint16_t, int) {
        unsubscribe_instant_action_finished_promise.set_value();
      });
  unsubscribe_instant_action_finished_promise.get_future().wait();

  connection_->Unsubscribe(rx_order.topic_name.c_str(), [&](Mqtt::MqttConnection&, uint16_t, int) {
    unsubscribe_order_finished_promise.set_value();
  });
  unsubscribe_order_finished_promise.get_future().wait();
  // Disconnect
  if (connection_->Disconnect()) {
    connection_closed_promise_.get_future().wait();
  }
}

template <>
void ManagerFSM<>::stop() {
  disconnect();
  stop_.store(true);
  io_context_->stop();
  worker_thread_.join();
}

template <>
bool ManagerFSM<>::filesExist(std::vector<std::string> filePaths) {
  for (const auto& path : filePaths) {
    // Check if the specified file exists.
    if (!filesys::exists(path)) {
      return false;
    }
  }
  return true;
}

template <>
optional<std::shared_ptr<Mqtt::MqttConnection>> ManagerFSM<>::initializeTLS() {
  if (!filesExist({config_.root_ca_path, config_.cert_path, config_.priv_key_path,
          config_.client_id_path})) {
    logger_->logError("Required files do not exist.");
    return nullopt;
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

  clientConfigBuilder.WithPortOverride(static_cast<uint16_t>(8883));

  // Create the MQTT connection from the MQTT builder
  auto clientConfig = clientConfigBuilder.Build();
  if (!clientConfig) {
    logger_->logError("Client Configuration initialization failed with error \n" +
                      std::string(ErrorDebugString(clientConfig.LastError())));
    return nullopt;
  }
  client_ = Aws::Iot::MqttClient();
  auto connection = client_.NewConnection(clientConfig);
  if (!*connection) {
    logger_->logError("MQTT Connection Creation failed with error \n" +
                      std::string(ErrorDebugString(connection->LastError())));
    return nullopt;
  }
  return connection;
}

}  // namespace impl
}  // namespace vda5050_connector