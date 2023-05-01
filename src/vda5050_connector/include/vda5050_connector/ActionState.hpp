#pragma once
#include "vda5050_connector/Action.hpp"
#include "vda5050_connector_interface/BaseActionState.hpp"

namespace vda5050_connector {
namespace impl {
const std::vector<std::string> validActionStates{interface::ActionStates::RUNNING,
    interface::ActionStates::WAITING, interface::ActionStates::FINISHED,
    interface::ActionStates::FAILED, interface::ActionStates::INITIALIZING,
    interface::ActionStates::PAUSED};
class ActionState : public interface::BaseActionState<Action> {
 public:
  bool operator==(const ActionState& as) const;
  void fromAction(const Action& a) override;
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
