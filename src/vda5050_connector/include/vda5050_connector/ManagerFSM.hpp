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

enum class SupportedTopic { STATE, ORDER, INSTANT_ACTION, CONNECTION, VISUALIZATION, FACT_SHEET };

template <class Order = Order, class InstantAction = InstantAction, class State = State,
    class Visualization = Visualization, class Connection = Connection, class FactSheet = FactSheet>
class ManagerFSM : public interface::BaseManagerInterface<Order, InstantAction, State,
                       Visualization, Connection, FactSheet> {
 public:
  ManagerFSM(interface::BaseNetworkConfiguration config);
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

  void setupPublisher(boost::asio::steady_timer& timer, const std::string& topic,
      const double& update_time, const std::function<void()>& func) override;
  void setPublishers(std::function<void()>& func){p_callback_ = func;};
  Order getOrderMsg() override;
  InstantAction getInstantActionMsg() override;
  void updateStateMsg(const std::function<void(State&)>& func) override;
  void updateVisualizationMsg(const std::function<void(Visualization&)>& func) override;
  void updateConnectionMsg(const std::function<void(Connection&)>& func) override;
  void updateFactSheetMsg(const std::function<void(FactSheet&)>& func) override;
  void registerSubscriber(std::string topic_name, const std::function<void()>& onReceive) override;
  void setOnOrderReceived(const std::function<void(Order&)>& func) { on_order_received_ = func; }
  void setOnInstantActionReceived(const std::function<void(InstantAction&)>& func) {
    on_instant_action_received_ = func;
  };

 protected:
  Aws::Crt::ApiHandle apiHandle;
  bool error_{false};
  std::atomic<bool> stop_{false};

  std::function<void()> p_callback_;
  std::function<void(Order&)> on_order_received_;
  std::function<void(InstantAction&)> on_instant_action_received_;

  std::shared_ptr<boost::asio::io_context> io_context_;
  boost::asio::steady_timer state_timer_;
  boost::asio::steady_timer visualization_timer_;
  boost::asio::steady_timer connection_timer_;
  boost::asio::steady_timer fact_sheet_timer_;
  boost::asio::steady_timer tick_timer_;

  std::thread worker_thread_;

  bool tls_initialized_{false};
  std::shared_ptr<Aws::Crt::Mqtt::MqttConnection> connection_;
  Aws::Iot::MqttClient client_;
  std::shared_ptr<iw::state_machine::StateMachine<vda5050_connector::interface::FSMState>>
      state_machine_;
  /// Message logger
  std::shared_ptr<iw::logging::StdLogger> logger_;
  std::promise<bool> connection_completed_promise_;
  std::promise<void> connection_closed_promise_;
  static const std::map<vda5050_connector::interface::FSMState, std::string> state_to_str_;

  std::string getISOCurrentTimestamp();
  void initializeStates() override;
  void initializeTransitions() override;
  void disconnect();
  std::experimental::optional<std::shared_ptr<Aws::Crt::Mqtt::MqttConnection>> initializeTLS();
  bool filesExist(std::vector<std::string> filePaths);
  void loadTopicInfoFromConfig(SupportedTopic t);
};
}  // namespace impl
}  // namespace vda5050_connector