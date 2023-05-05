#pragma once
#include <experimental/optional>

#include "vda5050_connector_interface/BaseTopicInterface.hpp"

namespace vda5050_connector {
namespace interface {

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
  std::string lastNodeId;
  nlohmann::json agvVersions;
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

  virtual std::experimental::optional<EdgeStateT> getEdgeStateWithId(
      const std::string& edgeId, const int& edgeSequenceId) = 0;
  virtual std::experimental::optional<NodeStateT> getNodeStateWithId(
      const std::string& nodeId, const int& nodeSequenceId) = 0;
  virtual std::experimental::optional<ActionStateT> getActionStateWithId(
      const std::string& actionId) = 0;
  virtual std::experimental::optional<NodeStateT> getNodeStateWithSequenceId(int sequenceId) = 0;
  virtual bool removeActionStateWithId(const std::string& actionId) = 0;
  virtual bool removeNodeStateWithId(const std::string& nodeId, const int& nodeSequenceId) = 0;
  virtual bool removeEdgeStateWithId(const std::string& edgeId, const int& edgeSequenceId) = 0;
  virtual bool updateContinuesOnPreviousOrder(const OrderT& orderUpdate) = 0;
  virtual bool hasHorizon() = 0;
  virtual bool hasActiveOrder() const = 0;
  virtual int getNumberOfReleasedNodes() = 0;
  virtual void updateEdgeStateTrajectory(
      const std::string& edgeId, const int& edgeSequenceId, TrajectoryT traj) = 0;

  virtual void updateNodeStateWithId(
      const std::string& nodeId, const int& nodeSequenceId, const NodeT& nodeUpdate) = 0;
  virtual void updateEdgeStateWithId(
      const std::string& edgeId, const int& edgeSequenceId, const EdgeT& edgeUpdate) = 0;
  virtual void updateActionStateWithId(
      const std::string& actionId, const std::string& newStatus) = 0;
  virtual void updateActionResultWithId(const std::string& actionId, const std::string& result) = 0;
};
}  // namespace interface
}  // namespace vda5050_connector