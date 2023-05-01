#pragma once
#include "vda5050_connector/Edge.hpp"
#include "vda5050_connector/Trajectory.hpp"
#include "vda5050_connector_interface/BaseEdgeState.hpp"

namespace vda5050_connector {
namespace impl {
class EdgeState : public interface::BaseEdgeState<Trajectory> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  void fromEdge(const Edge& edge);
  bool isEqualWithoutTrajectory(const EdgeState& es) const;
};
}  // namespace impl
}  // namespace vda5050_connector
