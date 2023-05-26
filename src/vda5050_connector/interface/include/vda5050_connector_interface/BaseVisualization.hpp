#pragma once
#if __cplusplus < 201703L
#include <experimental/optional>
namespace std {
using optional = experimental::optional;
}
#else
#include <optional>
#endif
#include "vda5050_connector_interface/BaseTopicInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class HeaderT, class EdgeStateT, class NodeStateT, class ActionStateT,
    class BatteryStateT, class ErrorT, class LoadT, class InfoT, class SafetyStateT,
    class AgvPositionT, class VelocityT>
class BaseVisualization : public BaseTopicInterface<HeaderT> {
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