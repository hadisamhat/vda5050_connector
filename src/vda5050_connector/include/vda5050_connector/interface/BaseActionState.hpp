#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {
namespace ActionStates {
// Action was triggered, preparatory measures are initiated.
constexpr char INITIALIZING[] = "INITIALIZING";

// The action is running.
constexpr char RUNNING[] = "RUNNING";

// The action is paused because of a pause instantAction or external trigger.
constexpr char PAUSED[] = "PAUSED";

/* Action was received by AGV but the node
where it triggers was not yet reached or the
edge where it is active was not yet entered. */
constexpr char WAITING[] = "WAITING";

// The action is finished.
constexpr char FINISHED[] = "FINISHED";

// Action could not be finished.
constexpr char FAILED[] = "FAILED";
};  // namespace ActionStates

template <class ActionT, class NodePositionT>
class BaseActionState : public BaseInterface {
 public:
  std::string actionId;
  std::string actionType;
  std::string actionDescription;  // OPTIONAL
  std::string actionStatus;
  std::string resultDescription;  // OPTIONAL

  virtual void fromAction(const ActionT& a) = 0;
};

}  // namespace interface
}  // namespace vda5050_connector
