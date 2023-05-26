#include <csignal>

#include "vda5050_connector/Connection.hpp"
#include "vda5050_connector/FactSheet.hpp"
#include "vda5050_connector/InstantAction.hpp"
#include "vda5050_connector/ManagerFSM.hpp"
#include "vda5050_connector/Order.hpp"
#include "vda5050_connector/State.hpp"
#include "vda5050_connector/Visualization.hpp"

using namespace vda5050_connector::impl;
using namespace vda5050_connector::interface;

std::atomic<bool> ctrlCPressed(false);  // Atomic flag to indicate whether Ctrl+C is pressed

// Signal handler for SIGINT (Ctrl+C)
void signalHandler(int signal) {
  if (signal == SIGINT) {
    ctrlCPressed = true;  // Set the flag to true when Ctrl+C is pressed
  }
}

int main() {
  std::signal(SIGINT, signalHandler);
  BaseNetworkConfiguration config;
  config.cert_path = "/home/user/device_certificates/certificate.pem.crt";
  config.client_id_path = "/home/user/device_certificates/client_id.txt";
  config.connection_state_interval_secs = 2;
  config.state_interval_secs = 2;
  config.visualization_interval_secs = 5;
  config.fact_sheet_interval_secs = 2;
  config.mode = "qa";
  config.qa_topic_prefix = "qa";
  config.dev_topic_prefix = "dev";
  config.priv_key_path = "/home/user/device_certificates/private.pem.key";
  config.root_ca_path = "/home/user/bootstrap_certificates/rootCA.crt";
  config.min_reconnect_backoff_sec = 0.01;
  config.max_reconnect_backoff_sec = 0.01;
  config.protocol_version = "5.0.0";
  config.instant_action_topic_name = "instantAction";
  config.order_topic_name = "order";
  config.visualization_topic_name = "viz";
  config.connection_topic_name = "con";
  config.fact_sheet_topic_name = "fs";
  config.state_topic_name = "state";

  boost::asio::io_context ioContext;
  auto manager = std::make_shared<
      ManagerFSM<Order, InstantAction, State, Visualization, Connection, FactSheet>>(
      config, ioContext);
  manager->setOnOrderReceived([&](Order action) {
    std::cout << "Custom on received function executed" << std::endl;
    std::cout << "Action type received " << action.orderId << std::endl;
  });
  std::cout << "before start";
  manager->start();
  std::cout << "after start";
  auto worker_thread_ = std::thread([&ioContext]() { ioContext.run(); });
  std::cout << "after thread";

  while (!ctrlCPressed) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  };
  std::cout << "control c pressed";
  worker_thread_.join();
  manager->stop();
  return 0;
}
