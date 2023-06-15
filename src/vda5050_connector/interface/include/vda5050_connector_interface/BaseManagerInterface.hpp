#pragma once
#include <atomic>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <memory>
#include <mutex>
#include <thread>

#include "vda5050_connector_interface/BaseNetworkConfiguration.hpp"
#include "vda5050_connector_interface/BasePublishedTopic.hpp"
#include "vda5050_connector_interface/BaseSubscribedTopic.hpp"
#include "vda5050_connector_interface/Vda5050Exception.hpp"

namespace vda5050_connector {
namespace interface {
enum class FSMState { INIT, OPERATIONAL, DISCONNECTED, ERROR };
template <class Order, class InstantAction, class State, class Visualization, class Connection,
    class FactSheet>
class BaseManagerInterface {
 public:
  /// Starts the state machine
  virtual void start() = 0;
  /// Return current state of fsm
  virtual FSMState getCurrentState() = 0;
  virtual std::string getName() const = 0;
  /// Returns the name of the current state of the FSM
  virtual std::string getCurrentStateName() = 0;
  /// Stops the FSM
  virtual void stop() = 0;

  virtual void setupPublisher(boost::asio::steady_timer& timer, const std::string& topic,
      const double& update_time, const std::function<void()>& func) = 0;

  virtual Order getOrderMsg() = 0;
  virtual InstantAction getInstantActionMsg() = 0;

  virtual void setOnOrderReceived(const std::function<void(Order&)>& func) = 0;
  virtual void setOnInstantActionReceived(const std::function<void(InstantAction&)>& func) = 0;

  virtual void setOnInstantActionJsonReceived(
      const std::function<void(BaseSubscribedTopic<InstantAction>&)>& func) = 0;
  virtual void setOnOrderJsonReceived(
      const std::function<void(BaseSubscribedTopic<Order>&)>& func) = 0;

  virtual void updateStateMsg(const std::function<void(State&)>& func) = 0;
  virtual void updateVisualizationMsg(const std::function<void(Visualization&)>& func) = 0;
  virtual void updateConnectionMsg(const std::function<void(Connection&)>& func) = 0;
  virtual void updateFactSheetMsg(const std::function<void(FactSheet&)>& func) = 0;

 protected:
  /// Ticks the state machine
  virtual void tick() = 0;
  /// Initializes the state machine
  virtual void createStateMachine() = 0;
  /// Associates each State with its description string
  virtual void initializeStates() = 0;       ///< Initializes the states of the machine
  virtual void initializeTransitions() = 0;  ///< Initializes the transitions of the machine
                                             /// The state machine used to track the current state
};
}  // namespace interface
}  // namespace vda5050_connector