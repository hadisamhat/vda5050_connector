#pragma once
#include <experimental/optional>

#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class EdgeStateT, class NodeStateT, class ActionStateT, class BatteryStateT, class ErrorT,
    class LoadT, class InfoT, class SafetyStateT, class AgvPositionT, class VelocityT>
class BaseVisualization : public BaseInterface {
 public:
  std::string orderId;
  int orderUpdateId;
  std::string lastNodeId;
  int lastNodeSequenceId;
  std::vector<NodeStateT> nodeStates;
  std::vector<EdgeStateT> edgeStates;
  AgvPositionT agvPosition;  // OPTIONAL
  VelocityT velocity;        // OPTIONAL
  bool driving;
  bool paused;  // OPTIONAL
  bool newBaseRequest;
  float distanceSinceLastNode;  // OPTIONAL
  std::vector<ActionStateT> actionStates;
  BatteryStateT batteryState;
  std::string operatingMode;
  std::vector<ErrorT> errors;
  std::vector<LoadT> loads;
  std::vector<InfoT> informations;
  SafetyStateT safState;
};
}  // namespace interface
}  // namespace vda5050_connector