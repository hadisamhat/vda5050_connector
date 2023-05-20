#include <atomic>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <memory>
#include <mutex>
#include <thread>

#include "vda5050_connector_interface/BaseNetworkConfiguration.hpp"
#include "vda5050_connector_interface/BasePublishedTopic.hpp"
#include "vda5050_connector_interface/BaseSubscribedTopic.hpp"

namespace vda5050_connector {
namespace interface {
enum class FSMState { INIT, OPERATIONAL, CONNECTED, DISCONNECTED, ERROR };
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
  BaseSubscribedTopic<Order> rx_order;
  BaseSubscribedTopic<InstantAction> rx_instant_action;
  BasePublishedTopic<State> tx_state;
  BasePublishedTopic<Visualization> tx_visualization;
  BasePublishedTopic<Connection> tx_connection;
  BasePublishedTopic<FactSheet> tx_fact_sheet;

 protected:
  /// Ticks the state machine
  virtual void tick() = 0;
  /// Initializes the state machine
  virtual void createStateMachine() = 0;
  /// Associates each State with its description string
  virtual void initializeStates() = 0;       ///< Initializes the states of the machine
  virtual void initializeTransitions() = 0;  ///< Initializes the transitions of the machine
                                             /// The state machine used to track the current state
  std::shared_ptr<boost::asio::io_context> io_context_;
  std::thread worker_thread_;
  BaseNetworkConfiguration config_;
  std::string client_id_;
};
}  // namespace interface
}  // namespace vda5050_connector