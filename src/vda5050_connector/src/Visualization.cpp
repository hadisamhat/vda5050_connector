#include "vda5050_connector/Visualization.hpp"

using namespace std;
using Json = nlohmann::json;
using namespace std;
using std::nullopt;
using std::optional;

namespace vda5050_connector {
namespace impl {

Json Visualization::to_json() {
  Json j = Json{{"orderId", this->orderId}, {"orderUpdateId", this->orderUpdateId},
      {"lastNodeId", this->lastNodeId}, {"lastNodeId", this->lastNodeId},
      {"lastNodeSequenceId", this->lastNodeSequenceId},
      {"agvPosition", this->agvPosition.to_json()}, {"velocity", this->velocity.to_json()},
      {"driving", this->driving}, {"paused", this->paused},
      {"newBaseRequest", this->newBaseRequest},
      {"distanceSinceLastNode", this->distanceSinceLastNode},
      {"batteryState", this->batteryState.to_json()}, {"operatingMode", this->operatingMode},
      {"safetyState", this->safState.to_json()}};
  Json nodeStates_j = Json::array();
  for (auto& n : this->nodeStates) {
    nodeStates_j.push_back(n.to_json());
  }
  j["nodeStates"] = nodeStates_j;
  Json edgeStates_j = Json::array();
  for (auto& e : this->edgeStates) {
    edgeStates_j.push_back(e.to_json());
  }
  j["edgeStates"] = edgeStates_j;
  Json actionStates_j = Json::array();
  for (auto& a : this->actionStates) {
    actionStates_j.push_back(a.to_json());
  }
  j["actionStates"] = actionStates_j;
  Json errors_j = Json::array();
  for (auto& e : this->errors) {
    errors_j.push_back(e.to_json());
  }
  j["errors"] = errors_j;
  Json loads_j = Json::array();
  for (auto& n : this->loads) {
    loads_j.push_back(n.to_json());
  }
  j["loads"] = loads_j;
  Json informations_j = Json::array();
  for (auto& i : this->informations) {
    informations_j.push_back(i.to_json());
  }
  j["informations"] = informations_j;
  return j;
}

void Visualization::from_json(const Json& j) {
  j.at("orderId").get_to(this->orderId);
  j.at("orderUpdateId").get_to(this->orderUpdateId);
  j.at("lastNodeId").get_to(this->lastNodeId);
  j.at("lastNodeSequenceId").get_to(this->lastNodeSequenceId);
  j.at("paused").get_to(this->paused);
  j.at("driving").get_to(this->driving);
  j.at("newBaseRequest").get_to(this->newBaseRequest);
  j.at("operatingMode").get_to(this->operatingMode);
  j.at("distanceSinceLastNode").get_to(this->distanceSinceLastNode);
  this->agvPosition.from_json(j.at("agvPosition"));
  this->velocity.from_json(j.at("velocity"));
  this->batteryState.from_json(j.at("batteryState"));
  this->safState.from_json(j.at("safetyState"));
  for (const auto& j : j.at("nodeStates")) {
    NodeState n;
    n.from_json(j);
    this->nodeStates.push_back(n);
  }
  for (const auto& j : j.at("edgeStates")) {
    EdgeState e;
    e.from_json(j);
    this->edgeStates.push_back(e);
  }
  for (const auto& j : j.at("actionStates")) {
    ActionState as;
    as.from_json(j);
    this->actionStates.push_back(as);
  }
  for (const auto& j : j.at("errors")) {
    Error e;
    e.from_json(j);
    this->errors.push_back(e);
  }
  for (const auto& j : j.at("loads")) {
    LoadSet ls;
    ls.from_json(j);
    this->loads.push_back(ls);
  }
  for (const auto& j : j.at("informations")) {
    Info i;
    i.from_json(j);
    this->informations.push_back(i);
  }
}
}  // namespace impl
}  // namespace vda5050_connector