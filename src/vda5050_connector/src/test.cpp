#include <csignal>

#include "vda5050_connector/ManagerFSM.hpp"

using namespace vda5050_connector::impl;
using namespace vda5050_connector::interface;

volatile std::sig_atomic_t gSignalStatus = 0;

// Signal handler for SIGINT (Ctrl+C)
void signalHandler(int signal) { gSignalStatus = signal; }

int main() {
  std::signal(SIGINT, signalHandler);
  MQTTConfiguration config;
  config.cert_path = "/home/user/ros2_ws/device_certificates/certificate.pem.crt";
  config.client_id_path = "/home/user/ros2_ws/device_certificates/client_id.txt";
  config.connection_state_interval_secs = 0.05;
  config.state_interval_secs = 0.05;
  config.visualization_interval_secs = 0.05;
  config.factsheet_interval_secs = 0.05;
  config.endpoint = "ahmj34rbzc71v-ats.iot.eu-central-1.amazonaws.com";
  config.priv_key_path = "/home/user/ros2_ws/device_certificates/private.pem.key";
  config.root_ca_path = "/home/user/ros2_ws/bootstrap_certificates/rootCA.crt";
  config.min_reconnect_backoff_sec = 0.01;
  config.max_reconnect_backoff_sec = 0.01;
  config.protocol_version = "5.0.0";
  config.manufacturer = "BMW - AG";
  config.serial_number = "1423320013919";

  std::shared_ptr<ManagerFSM> manager = std::make_shared<ManagerFSM>(config);
  manager->rx_instant_action.topic_name = "qa/QA_1423320013919/instantAction";
  manager->rx_order.topic_name = "qa/QA_1423320013919/order";
  manager->tx_state.update_time_s = 5;
  manager->tx_visualization.update_time_s = 5;
  manager->tx_connection.update_time_s = 5;
  manager->tx_fact_sheet.update_time_s = 5;
  manager->tx_state.msg.operatingMode = "a";
  manager->tx_state.updatePublisher([&](State& s) {
    std::cout << "before update " << s.operatingMode;
    s.operatingMode = "sadasdsa";
    std::cout << " after update " << s.operatingMode;
  });
  manager->start();
  while (gSignalStatus == 0) continue;
  manager->stop();
  return 0;
}