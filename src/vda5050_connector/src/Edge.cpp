#include "vda5050_connector/Edge.hpp"

using namespace boost::algorithm;
using namespace std;
using std::nullopt;
using std::optional;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

std::optional<Action> Edge::tryGetActionOfType(const std::string& action_type) {
  auto it = find_if(
      actions.begin(), actions.end(), [&](const Action& a) { return a.actionType == action_type; });
  if (it == actions.end()) return std::nullopt;
  return *it;
}
Json Edge::to_json() {
  Json j = Json{{"edgeId", this->edgeId}, {"sequenceId", this->sequenceId},
      {"edgeDescritption", this->edgeDescription}, {"released", this->released},
      {"startNodeId", this->startNodeId}, {"endNodeId", this->endNodeId},
      {"maxSpeed", this->maxSpeed}, {"maxHeight", this->maxHeight}, {"minHeight", this->minHeight},
      {"orientation", this->orientation}, {"direction", this->direction},
      {"rotationAllowed", this->rotationAllowed}, {"maxRotationSpeed", this->maxRotationSpeed},
      {"trajectory", this->traj.to_json()}, {"length", this->length}};
  Json action_j = Json::array();
  for (auto& a : this->actions) {
    action_j.push_back(a.to_json());
  }
  j["actions"] = action_j;
  return j;
}
void Edge::from_json(const Json& j) {
  j.at("edgeId").get_to(this->edgeId);
  j.at("sequenceId").get_to(this->sequenceId);
  if (j.find("edgeDescription") != j.end() && !j.at("edgeDescription").is_null()) {
    j.at("edgeDescription").get_to(this->edgeDescription);
  }
  j.at("released").get_to(this->released);
  j.at("startNodeId").get_to(this->startNodeId);
  j.at("endNodeId").get_to(this->endNodeId);
  if (j.find("maxSpeed") != j.end() && !j.at("maxSpeed").is_null()) {
    j.at("maxSpeed").get_to(this->maxSpeed);
  }
  if (j.find("maxHeight") != j.end() && !j.at("maxHeight").is_null()) {
    j.at("maxHeight").get_to(this->maxHeight);
  }
  if (j.find("minHeight") != j.end() && !j.at("minHeight").is_null()) {
    j.at("minHeight").get_to(this->minHeight);
  }
  if (j.find("orientation") != j.end() && !j.at("orientation").is_null()) {
    j.at("orientation").get_to(this->orientation);
  }
  if (j.find("direction") != j.end() && !j.at("direction").is_null()) {
    j.at("direction").get_to(this->direction);
  }
  if (j.find("rotationAllowed") != j.end() && !j.at("rotationAllowed").is_null()) {
    j.at("rotationAllowed").get_to(this->rotationAllowed);
  }
  if (j.find("maxRotationSpeed") != j.end() && !j.at("maxRotationSpeed").is_null()) {
    j.at("maxRotationSpeed").get_to(this->maxRotationSpeed);
  }
  if (j.find("trajectory") != j.end() && !j.at("trajectory").is_null()) {
    this->traj.from_json(j.at("trajectory"));
  }
  if (j.find("length") != j.end() && !j.at("length").is_null()) {
    j.at("length").get_to(this->length);
  }
  for (const auto& j : j.at("actions")) {
    Action a;
    a.from_json(j);
    this->actions.push_back(a);
  }
}

void Edge::fromEdge(const Edge& e) {
  this->edgeId = e.edgeId;
  this->sequenceId = e.sequenceId;
  this->edgeDescription = e.edgeDescription;
  this->traj = e.traj;
  this->released = e.released;
  this->actions = e.actions;
}

bool Edge::hasActionOfType(const std::string& action_type) const {
  auto it = find_if(
      actions.begin(), actions.end(), [&](const Action& a) { return a.actionType == action_type; });
  return it != actions.end();
}
}  // namespace impl
}  // namespace vda5050_connector