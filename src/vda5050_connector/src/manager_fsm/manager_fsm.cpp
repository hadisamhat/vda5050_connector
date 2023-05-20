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

ManagerFSM::ManagerFSM(BaseNetworkConfiguration config) {
  config_ = config;
  logger_ = std::make_shared<iw::logging::StdLogger>();
  state_machine_ =
      std::make_shared<iw::state_machine::StateMachine<vda5050_connector::interface::FSMState>>(
          getName(), logger_);
  io_context_ = std::make_shared<boost::asio::io_context>();
};

void ManagerFSM::loadTopicInfoFromConfig(SupportedTopic t) {
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
std::string ManagerFSM::getISOCurrentTimestamp() {
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

void ManagerFSM::createStateMachine() {
  state_machine_->setToString(this->getStateName);
  initializeStates();
  initializeTransitions();
}

void ManagerFSM::tick() {
  if (stop_.load()) return;
  state_machine_->tick();
  io_context_->post([this]() { tick(); });
}

void ManagerFSM::start() {
  createStateMachine();
  state_machine_->start(FSMState::INIT);
  worker_thread_ = std::thread([this]() {
    boost::asio::executor_work_guard<decltype(io_context_->get_executor())> work{
        io_context_->get_executor()};
    io_context_->run();
  });
  io_context_->post([&]() { tick(); });
}

void ManagerFSM::disconnect() {
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
void ManagerFSM::stop() {
  disconnect();
  stop_.store(true);
  io_context_->stop();
  worker_thread_.join();
}

bool ManagerFSM::filesExist(std::vector<std::string> filePaths) {
  for (const auto& path : filePaths) {
    // Check if the specified file exists.
    if (!filesys::exists(path)) {
      return false;
    }
  }
  return true;
}

optional<std::shared_ptr<Mqtt::MqttConnection>> ManagerFSM::initializeTLS() {
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