#include "iw_vda5050_connector/Vda5050Manager.hpp"
using namespace Aws::Crt;
using Json = nlohmann::json;

namespace iw {
namespace vda5050 {
Manager::Manager(NetworkConfiguration config)
    : ManagerFSM<Order, InstantAction, State, Visualization, Connection, FactSheet>(config) {
  config_ = config;
  tx_fact_sheet_.enable = false;
}

std::string Manager::getTopicFromString(std::string topic_name) {
  std::string prefix;

  if (config_.mode == "dev") {
    prefix = config_.dev_topic_prefix + "/";
  } else if (config_.mode == "qa") {
    prefix = config_.qa_topic_prefix + "/";
  }
  return prefix.append(client_id_).append("/").append(topic_name);
}

MapUpdate Manager::getMapUpdateMsg() {
  std::lock_guard<std::mutex> lock(rx_map_update_.sub_mutex);
  return rx_map_update_.msg;
}

ZoneUpdate Manager::getZoneUpdateMsg() {
  std::lock_guard<std::mutex> lock(rx_zone_update_.sub_mutex);
  return rx_zone_update_.msg;
}

void Manager::start() {
  createStateMachine();
  state_machine_->start(FSMState::INIT);
  tick();
  worker_thread_ = std::thread([this]() { io_context_->run(); });
  registerSubscribers();
}

void Manager::registerSubscribers() {
  /// map update subscriber
  rx_map_update_.topic_name = getTopicFromString(config_.map_update_topic_name);
  registerSubscriber(rx_map_update_.topic_name, [this](Json j) {
    if (!on_map_update_received_) return;
    std::lock_guard<std::mutex> lock(rx_map_update_.sub_mutex);
    rx_map_update_.msg.from_json(j);
    on_map_update_received_(rx_map_update_.msg);
  });

  rx_zone_update_.topic_name = getTopicFromString(config_.zone_update_topic_name);
  registerSubscriber(rx_zone_update_.topic_name, [this](Json j) {
    if (!on_map_update_received_) return;
    std::lock_guard<std::mutex> lock(rx_zone_update_.sub_mutex);
    rx_zone_update_.msg.from_json(j);
    on_zone_update_received_(rx_zone_update_.msg);
  });

  rx_ssh_token_.topic_name =
      std::regex_replace(DEFAULT_SSH_TUNNEL_TOPIC, std::regex("CLIENT_ID"), client_id_);
  registerSubscriber(rx_ssh_token_.topic_name, [this](Json j) {
    if (!on_ssh_token_received_) return;
    on_ssh_token_received_(j);
  });
}
}  // namespace vda5050
}  // namespace iw