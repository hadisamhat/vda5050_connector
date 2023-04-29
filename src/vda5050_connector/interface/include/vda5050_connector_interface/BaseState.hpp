#pragma once
#include <experimental/optional>

#include "vda5050_connector_interface/BaseTopicInterface.hpp"

namespace iw {
namespace vda5050_connector_interface {

namespace OperatingModes {
constexpr char AUTOMATIC[] = "AUTOMATIC";
constexpr char SEMIAUTOMATIC[] = "SEMIAUTOMATIC";
constexpr char MANUAL[] = "MANUAL";
constexpr char SERVICE[] = "SERVICE";
constexpr char TEACHIN[] = "TEACHIN";
};  // namespace OperatingModes
template <class HeaderT, class OrderT, class EdgeT, class NodeT, class EdgeStateT, class NodeStateT,
    class ActionStateT, class BatteryStateT, class ErrorT, class LoadT, class InfoT,
    class TrajectoryT, class SafetyStateT, class AgvPositionT, class VelocityT>
class BaseState : public BaseTopicInterface<HeaderT> {
 public:
  std::string orderId;
  int orderUpdateId;
  std::string zoneSetId;  // OPTIONAL
  std::string lastNodeId;
  int lastNodeSequenceId;
  std::vector<NodeStateT> nodeStates;
  std::vector<EdgeStateT> edgeStates;
  AgvPositionT agvPosition;  // OPTIONAL
  VelocityT velocity;        // OPTIONAL
  bool driving;
  bool paused;                  // OPTIONAL
  float distanceSinceLastNode;  // OPTIONAL, Unit : meters
  bool newBaseRequest;          // OPTIONAL
  std::vector<ActionStateT> actionStates;
  BatteryStateT batteryState;
  std::string operatingMode;
  std::vector<ErrorT> errors;
  std::vector<LoadT> loads;
  std::vector<InfoT> informations;
  SafetyStateT safState;

  std::experimental::optional<EdgeStateT> getEdgeStateWithId(
      const std::string& edgeId, const int& edgeSequenceId);
  std::experimental::optional<NodeStateT> getNodeStateWithId(
      const std::string& nodeId, const int& nodeSequenceId);
  std::experimental::optional<ActionStateT> getActionStateWithId(const std::string& actionId);
  std::experimental::optional<NodeStateT> getNodeStateWithSequenceId(int sequenceId);
  bool removeActionStateWithId(const std::string& actionId);
  bool removeNodeStateWithId(const std::string& nodeId, const int& nodeSequenceId);
  bool removeEdgeStateWithId(const std::string& edgeId, const int& edgeSequenceId);
  bool updateContinuesOnPreviousOrder(const OrderT& orderUpdate);
  bool hasHorizon();
  bool hasActiveOrder() const;
  int getNumberOfReleasedNodes();
  void updateEdgeStateTrajectory(
      const std::string& edgeId, const int& edgeSequenceId, TrajectoryT traj);

  void updateNodeStateWithId(
      const std::string& nodeId, const int& nodeSequenceId, const NodeT& nodeUpdate);
  void updateEdgeStateWithId(
      const std::string& edgeId, const int& edgeSequenceId, const EdgeT& edgeUpdate);
  void updateActionStateWithId(const std::string& actionId, const std::string& newStatus);
  void updateActionResultWithId(const std::string& actionId, const std::string& result);
};
}  // namespace vda5050_connector_interface
}  // namespace iw