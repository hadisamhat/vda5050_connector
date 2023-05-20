#pragma once
#include <aws/crt/Api.h>
#include <aws/crt/StlAllocator.h>
#include <aws/crt/UUID.h>
#include <aws/crt/auth/Credentials.h>
#include <aws/crt/io/TlsOptions.h>
#include <aws/iot/MqttClient.h>

#include <algorithm>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>
#include <condition_variable>
#include <cstdarg>
#include <cstring>
#include <experimental/optional>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

#include "iw_logging/std_logger.hpp"
#include "iw_state_machine/state_machine.hpp"
#include "vda5050_connector/Connection.hpp"
#include "vda5050_connector/FactSheet.hpp"
#include "vda5050_connector/InstantAction.hpp"
#include "vda5050_connector/Order.hpp"
#include "vda5050_connector/State.hpp"
#include "vda5050_connector/Visualization.hpp"
#include "vda5050_connector_interface/BaseManagerInterface.hpp"

namespace vda5050_connector {
namespace impl {
class ManagerFSM : public interface::BaseManagerInterface<Order, InstantAction, State,
                       Visualization, Connection, FactSheet> {
 public:
  ManagerFSM(MQTTConfiguration config);
  void createStateMachine() override;
  void start() override;
  void tick() override;
  void stop() override;
  std::string getName() const override { return "AWSManagerFSM"; }
  std::string getCurrentStateName() override { return state_machine_->getCurrentStateName(); }
  vda5050_connector::interface::FSMState getCurrentState() override {
    if (state_machine_->current_state())
      return state_machine_->current_state().value();
    else
      return vda5050_connector::interface::FSMState::ERROR;
  };
  static std::string getStateName(const vda5050_connector::interface::FSMState& state) {
    try {
      return state_to_str_.at(state);
    } catch (std::out_of_range& exception) {
      return std::string{"UNDEF_STATE_NAME"};
    }
  }

 private:
  Aws::Crt::ApiHandle apiHandle;
  bool error_{false};
  std::atomic<bool> stop_{false};
  std::mutex mtx;
  std::condition_variable cv;
  std::chrono::system_clock::time_point start_time_, state_update_time_;
  bool tls_initialized_{false};
  MQTTConfiguration config_;
  std::string getISOCurrentTimestamp();
  void initializeStates() override;
  void initializeTransitions() override;
  std::experimental::optional<std::shared_ptr<Aws::Crt::Mqtt::MqttConnection>> initializeTLS();
  bool filesExist(std::vector<std::string> filePaths);

  std::shared_ptr<Aws::Crt::Mqtt::MqttConnection> connection_;
  Aws::Iot::MqttClient client_;
  std::shared_ptr<iw::state_machine::StateMachine<vda5050_connector::interface::FSMState>>
      state_machine_;
  /// Message logger
  std::shared_ptr<iw::logging::StdLogger> logger_;
  std::promise<bool> connection_completed_promise_;
  std::promise<void> connection_closed_promise_;
  static const std::map<vda5050_connector::interface::FSMState, std::string> state_to_str_;
};
}  // namespace impl
}  // namespace vda5050_connector