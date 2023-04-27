#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

namespace ActionBlockingType {
// Action can be executed in parallel with other actions while also driving.
constexpr char NONE[] = "NONE";

// Action can be executed in parallel with other actions. Vehicle must not drive.
constexpr char SOFT[] = "SOFT";

// Action must not be executed in parallel with other actions.
constexpr char HARD[] = "HARD";

};  // namespace ActionBlockingType

template <class ActionParamT>
class BaseAction : public BaseInterface {
 public:
  std::string actionType;
  std::string actionDescription;
  std::string actionBlockingType;
  std::vector<ActionParamT> actionParameters;

  // Checks if the action is supported by validating the action with a map of valid actions.
  virtual bool isSupportedAction(const std::vector<std::string>& allowedActions) const = 0;
  // Checks if an action requires additional action parameters.
  virtual bool requiresActionParameters() const = 0;
  // Checks if an action contains the correct action parameters.
  virtual bool hasValidActionParameters() const = 0;
};
}  // namespace interface
}  // namespace vda5050_connector
