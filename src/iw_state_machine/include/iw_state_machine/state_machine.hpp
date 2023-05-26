/*
Copyright (c) 2018, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/
#pragma once

#include <chrono>
#if __cplusplus < 201703L
#include <experimental/optional>
namespace std {
using optional = experimental::optional
}
#else
#include <optional>
#endif
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "iw_logging/logger_base.hpp"

namespace iw {
namespace state_machine {

using std::optional;

/// A basic state machine.
///
/// The state machine is always in exactly one out of a set of user-define states. States are
/// enumerated by a template type chosen by the user.
///
/// For each state the user can specify three callback: entry, stay and exit. The entry function
/// is called whenever the state machine enters the state. The stay function is called each time
/// `tick` is called and the state machine stays in that state. The exit function is called whenever
/// the state machine exits the state.
///
/// In addition the user can add possible transitions between states. For each transition he can
/// specify a condition and a transition function. During each `tick` the condition functions of the
/// current state are evaluated and the first condition which is true will trigger a transition
/// into a new state.
template <typename StateId>
class StateMachine {
 public:
  using Condition = std::function<bool()>;  ///< Condition
  using Action = std::function<void()>;     ///< Action on condition

  /// Constructor
  StateMachine(const std::string& name, std::shared_ptr<logging::LoggerBase> logger)
      : name_(name), logger_(logger) {}

  /// Sets a function to convert a state identifier to a string
  void setToString(std::function<std::string(const StateId&)> to_string) { to_string_ = to_string; }

  /// Adds a new state and specifies the entry, stay and exit callbacks
  void addState(const StateId& state, Action&& on_entry_action, Action&& on_stay_action,
      Action&& on_exit_action) {
    if (states_.find(state) != states_.end()) {
      throw(std::runtime_error(std::string{"SM: Already added state "} + toString(state)));
    }
    states_[state] = State{std::forward<Action>(on_entry_action),
        std::forward<Action>(on_stay_action), std::forward<Action>(on_exit_action), {}};
  }
  /// Adds a new state with empty entry, stay and exit callbacks
  void addState(const StateId& state) {
    addState(
        state, [] {}, [] {}, [] {});
  }

  /// Adds a transition from `state_` to `state_2` and specifies the condition and transition
  /// callback functions.
  void addTransition(const StateId& state_1, const StateId& state_2, Condition&& condition,
      Action&& on_transition_action) {
    findState(state_1).transitions.emplace_back(Transition{
        state_2, std::forward<Condition>(condition), std::forward<Action>(on_transition_action)});
  }

  /// Initializes the state machine and enters the given state
  void start(const StateId& state) { enterState(state); }

  /// Stops the state machine (and leaves the current state; if any)
  void stop() { exitState(); }

  /// Ticks the state machine possibly calling callbacks
  void tick() {
    if (!current_state_) {
      throw(std::runtime_error("SM: Can't tick, not started or already stopped."));
    }
    auto& state = findState(*current_state_);
    for (auto& transition : state.transitions) {
      if (transition.condition()) {
        last_transition_time_ = std::chrono::steady_clock::now();
        if (logger_ && logging_enabled_)
          logger_->logInfo(name_ + " transitioning from state " + getCurrentStateName() + " to " +
                           toString(transition.target_state));
        exitState();
        transition.on_transition_action();
        enterState(transition.target_state);
        return;
      }
    }
    // No state transition thus we execute the stay action
    state.on_stay_action();
  }

  /// Gets the current state of the state machine
  optional<StateId> current_state() const { return current_state_; }

  /// Converts a state to a descriptive string
  std::string toString(const StateId& state_id) const {
    return to_string_ ? to_string_(state_id) : std::string("UNDEFINED NAME CONVERSION");
  }

  /// Returns the last transition time point
  std::chrono::steady_clock::time_point lastTransitionTime() const { return last_transition_time_; }

  /// True if the timeout for a given transition elapsed
  bool isTransitionTimeout(double timeout) const {
    return std::chrono::steady_clock::now() - last_transition_time_ >
           std::chrono::duration<double>(timeout);
  }

  /// Returns the name of the current state as a string
  std::string getCurrentStateName() const {
    return current_state_ ? toString(current_state_.value()) : std::string{"UNDEFINED STATE"};
  }

  /// Enables or disables logging of transitions
  void setLoggingEnabled(bool enabled) { logging_enabled_ = enabled; }

 private:
  /// A transition between two states
  struct Transition {
    StateId target_state;
    Condition condition;
    Action on_transition_action;
  };

  /// A state in which the state machine can be
  struct State {
    Action on_entry_action;
    Action on_stay_action;
    Action on_exit_action;
    std::vector<Transition> transitions;
  };

  /// Finds a state by identifier
  State& findState(const StateId& state_id) {
    auto it = states_.find(state_id);
    if (it == states_.end()) {
      throw(std::runtime_error(std::string{"SM: Unknown state " + toString(state_id)}));
    }
    return it->second;
  }

  /// Enters the given state assuming we are currently not in any state
  void enterState(const StateId& state_id) {
    if (current_state_) {
      throw(
          std::runtime_error(std::string{"SM: Can not enter state "} + toString(state_id) +
                             std::string{" because still in state "} + toString(*current_state_)));
    }
    findState(state_id).on_entry_action();
    findState(state_id).on_stay_action();
    current_state_ = state_id;
  }

  /// Exits the current state
  void exitState() {
    if (!current_state_) {
      throw(std::runtime_error("SM: Can not exit from state because not in a state"));
    }
    findState(*current_state_).on_exit_action();
    current_state_ = std::nullopt;
  }

  /// Function to convert a state identifier to a string
  std::function<std::string(const StateId&)> to_string_;

  /// Dictionary with all states. Transitions are stored as part of the source state.
  std::map<StateId, State> states_;

  /// The identifier of the current state of the state machine
  optional<StateId> current_state_;

  /// Holds the time point of the last transition
  std::chrono::steady_clock::time_point last_transition_time_;

  /// Name of the fsm
  std::string name_;

  /// To log messages
  std::shared_ptr<logging::LoggerBase> logger_;

  /// Enables or disables logging of transitions
  bool logging_enabled_{true};
};

}  // namespace state_machine
}  // namespace iw