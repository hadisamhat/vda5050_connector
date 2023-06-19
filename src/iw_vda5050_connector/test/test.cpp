#include <csignal>

#include "iw_vda5050_connector/Vda5050Manager.hpp"

using namespace iw::vda5050;
volatile std::sig_atomic_t gSignalStatus = 0;

// Signal handler for SIGINT (Ctrl+C)
void signalHandler(int signal) { gSignalStatus = signal; }

int main() {
  std::signal(SIGINT, signalHandler);
  NetworkConfiguration config;
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
  config.min_reconnect_backoff_sec = 2;
  config.max_reconnect_backoff_sec = 4;
  config.protocol_version = "5.0.0";
  config.instant_action_topic_name = "instantAction";
  config.order_topic_name = "order";
  config.visualization_topic_name = "viz";
  config.connection_topic_name = "con";
  config.fact_sheet_topic_name = "fs";
  config.state_topic_name = "state";
  config.map_update_topic_name = "map";
  config.zone_update_topic_name = "zones";
  config.ssh_topic_name = "ssh_key";
  config.manufacturer = "BMW - AG";
  config.serial_number = "1423320013919";
  config.endpoint = "ahmj34rbzc71v-ats.iot.eu-central-1.amazonaws.com";

  boost::asio::io_context io_context;
  shared_ptr<Manager> manager = make_shared<Manager>(config, io_context);
  manager->start();
  auto worker_thread_ = thread([&io_context]() {
    try {
      io_context.run();

    } catch (const std::exception& e) {
      std::cout << "Exception occurred: " << e.what() << std::endl;
    }
  });
  while (gSignalStatus == 0) {
    manager->updateStateMsg([&](State& state) {
      state.header.headerId = 5;
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  manager->stop();
  io_context.stop();
  worker_thread_.join();

  return 0;
}
