#pragma once
#include "vda5050_connector/ActionState.hpp"
#include "vda5050_connector/AgvPosition.hpp"
#include "vda5050_connector/BatteryState.hpp"
#include "vda5050_connector/Edge.hpp"
#include "vda5050_connector/EdgeState.hpp"
#include "vda5050_connector/Error.hpp"
#include "vda5050_connector/Info.hpp"
#include "vda5050_connector/LoadSet.hpp"
#include "vda5050_connector/Node.hpp"
#include "vda5050_connector/NodeState.hpp"
#include "vda5050_connector/Order.hpp"
#include "vda5050_connector/ProtocolHeader.hpp"
#include "vda5050_connector/SafetyState.hpp"
#include "vda5050_connector/Trajectory.hpp"
#include "vda5050_connector/Velocity.hpp"
#include "vda5050_connector_interface/BaseVisualization.hpp"

namespace vda5050_connector {
namespace impl {
class Visualization
    : public interface::BaseVisualization<ProtocolHeader, EdgeState, NodeState, ActionState,
          BatteryState, Error, LoadSet, Info, SafetyState, AgvPosition, Velocity> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector