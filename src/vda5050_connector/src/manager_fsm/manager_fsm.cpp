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

ManagerFSM::ManagerFSM(MQTTConfiguration config) : config_(config) {
  config_ = config;
  logger_ = std::make_shared<iw::logging::StdLogger>();
  state_machine_ =
      std::make_shared<iw::state_machine::StateMachine<vda5050_connector::interface::FSMState>>(
          getName(), logger_);
};

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
  io_context_.post([this]() { tick(); });
}

void ManagerFSM::start() {
  createStateMachine();
  state_machine_->start(FSMState::INIT);
  worker_thread_ = std::thread([this]() {
    io_context_.post([this]() { tick(); });
    io_context_.run();
  });
}

void ManagerFSM::stop() {
  stop_.store(true);
  if (!io_context_.stopped()) {
    io_context_.stop();
  }
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