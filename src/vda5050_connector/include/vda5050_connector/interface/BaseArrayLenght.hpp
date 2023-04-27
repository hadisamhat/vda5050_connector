#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseArrayLength : public BaseInterface {
 public:
  int orderNodes;
  int orderEdges;
  int nodeActions;
  int edgeActions;
  int actionsActionsParameters;
  int instantActions;
  int trajectoryKnotVector;
  int trajectoryControlPoints;
  int stateNodeStates;
  int stateEdgeStates;
  int stateLoads;
  int stateActionStates;
  int stateErrors;
  int stateInformation;
  int errorErrorReferences;
  int informationInfoReferences;
};

}  // namespace interface
}  // namespace vda5050_connector
