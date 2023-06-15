#pragma once
#include "iw_vda5050_connector/InteractionZoneState.hpp"
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
#include "vda5050_connector/SafetyState.hpp"
#include "vda5050_connector/Trajectory.hpp"
#include "vda5050_connector/Velocity.hpp"
#include "vda5050_connector_interface/BaseState.hpp"

using namespace vda5050_connector::impl;
namespace iw {
namespace vda5050 {
class State : public vda5050_connector::interface::BaseState<Order, Edge, Node, EdgeState,
                  NodeState, ActionState, BatteryState, Error, Load, Info, Trajectory, SafetyState,
                  AgvPosition, Velocity> {
 public:
  std::string zoneSetId;                               // OPTIONAL
  bool waitingForInteractionZoneRelease;               // TEMPORARY
  std::vector<InteractionZoneState> interactionZones;  // TEMPORARY

  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  std::optional<EdgeState> getEdgeStateWithId(
      const std::string& edgeId, int edgeSequenceId) override;
  std::optional<NodeState> getNodeStateWithId(
      const std::string& nodeId, int nodeSequenceId) override;
  std::optional<ActionState> getActionStateWithId(
      const std::string& actionId) override;
  std::optional<NodeState> getNodeStateWithSequenceId(int sequenceId) override;
  bool removeActionStateWithId(const std::string& actionId) override;
  bool removeNodeStateWithId(const std::string& nodeId, int nodeSequenceId) override;
  bool removeEdgeStateWithId(const std::string& edgeId, int edgeSequenceId) override;
  bool updateContinuesOnPreviousOrder(const Order& orderUpdate) override;
  bool hasHorizon() override;
  bool hasActiveOrder() const override;
  int getNumberOfReleasedNodes() override;
  void updateEdgeStateTrajectory(
      const std::string& edgeId, int edgeSequenceId, Trajectory traj) override;
  void updateNodeStateWithId(
      const std::string& nodeId, int nodeSequenceId, const Node& nodeUpdate) override;
  void updateEdgeStateWithId(
      const std::string& edgeId, int edgeSequenceId, const Edge& edgeUpdate) override;
  void updateActionStateWithId(const std::string& actionId, const std::string& newStatus) override;
  void updateActionResultWithId(const std::string& actionId, const std::string& result) override;
  bool isSignificantChange(const State& s);
};
}  // namespace vda5050
}  // namespace iw