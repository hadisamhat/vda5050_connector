#pragma once
#include "iw_vda5050_connector/Load.hpp"
#include "vda5050_connector/ActionState.hpp"
#include "vda5050_connector/AgvPosition.hpp"
#include "vda5050_connector/BatteryState.hpp"
#include "vda5050_connector/Edge.hpp"
#include "vda5050_connector/EdgeState.hpp"
#include "vda5050_connector/Error.hpp"
#include "vda5050_connector/Info.hpp"
#include "vda5050_connector/Node.hpp"
#include "vda5050_connector/NodeState.hpp"
#include "vda5050_connector/Order.hpp"
#include "vda5050_connector/ProtocolHeader.hpp"
#include "vda5050_connector/SafetyState.hpp"
#include "vda5050_connector/Trajectory.hpp"
#include "vda5050_connector/Velocity.hpp"
#include "vda5050_connector_interface/BaseVisualization.hpp"

using namespace vda5050_connector::impl;
namespace iw {
namespace vda5050 {
class Visualization
    : public vda5050_connector::interface::BaseVisualization<ProtocolHeader, EdgeState, NodeState,
          ActionState, BatteryState, Error, Load, Info, SafetyState, AgvPosition, Velocity> {
 public:
  bool waitingForInteractionZoneRelease;  // TEMPORARY
  std::string zoneSetId;                  // OPTIONAL
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace vda5050
}  // namespace iw