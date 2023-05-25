#pragma once
#include "vda5050_connector/Action.hpp"
#include "vda5050_connector/Trajectory.hpp"
#include "vda5050_connector_interface/BaseEdge.hpp"

namespace vda5050_connector {
namespace impl {
class Edge : public interface::BaseEdge<Trajectory, Action> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  void fromEdge(const Edge& edge);
  std::optional<Action> tryGetActionOfType(const std::string& action_type) override;
  bool hasActionOfType(const std::string& action_type) const override;
};
}  // namespace impl
}  // namespace vda5050_connector
