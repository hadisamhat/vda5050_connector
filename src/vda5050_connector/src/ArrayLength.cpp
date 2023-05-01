#include "vda5050_connector/ArrayLength.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void ArrayLength::from_json(const nlohmann::json& j) {
  j.at("orderNodes").get_to(this->orderNodes);
  j.at("orderEdges").get_to(this->orderEdges);
  j.at("nodeActions").get_to(this->nodeActions);
  j.at("edgeActions").get_to(this->edgeActions);
  j.at("actionsActionsParameters").get_to(this->actionsActionsParameters);
  j.at("trajectoryKnotVector").get_to(this->trajectoryKnotVector);
  j.at("trajectoryControlPoints").get_to(this->trajectoryControlPoints);
  j.at("stateNodeStates").get_to(this->stateNodeStates);
  j.at("stateEdgeStates").get_to(this->stateEdgeStates);
  j.at("stateLoads").get_to(this->stateLoads);
  j.at("stateActionStates").get_to(this->stateActionStates);
  j.at("stateErrors").get_to(this->stateErrors);
  j.at("stateInformation").get_to(this->stateInformation);
  j.at("errorErrorReferences").get_to(this->errorErrorReferences);
  j.at("informationInfoReferences").get_to(this->informationInfoReferences);
}
Json ArrayLength::to_json() {
  return Json{{"orderNodes", this->orderNodes}, {"orderEdges", this->orderEdges},
      {"nodeActions", this->nodeActions}, {"nodeActions", this->nodeActions},
      {"edgeActions", this->edgeActions},
      {"actionsActionsParameters", this->actionsActionsParameters},
      {"trajectoryKnotVector", this->trajectoryKnotVector},
      {"trajectoryControlPoints", this->trajectoryControlPoints},
      {"stateNodeStates", this->stateNodeStates}, {"stateEdgeStates", this->stateEdgeStates},
      {"stateLoads", this->stateLoads}, {"stateActionStates", this->stateActionStates},
      {"stateErrors", this->stateErrors}, {"stateInformation", this->stateInformation},
      {"errorErrorReferences", this->errorErrorReferences},
      {"informationInfoReferences", this->informationInfoReferences}};
}

}  // namespace impl
}  // namespace vda5050_connector