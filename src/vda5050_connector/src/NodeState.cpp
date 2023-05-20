#include "vda5050_connector/NodeState.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
Json NodeState::to_json() {
  return {{"nodeId", this->nodeId}, {"sequenceId", this->sequenceId},
      {"nodeDescription", this->nodeDescription}, {"nodePosition", this->nodePosition.to_json()},
      {"released", this->released}};
}

void NodeState::from_json(const Json& j) {
  j.at("nodeId").get_to(this->nodeId);
  j.at("sequenceId").get_to(this->sequenceId);
  if (j.find("nodeDescription") != j.end() && !j.at("nodeDescription").is_null()) {
    j.at("nodeDescription").get_to(this->nodeDescription);
  }
  j.at("released").get_to(this->released);
  this->nodePosition.from_json(j.at("nodePosition"));
}

bool NodeState::operator==(const NodeState& ns) const {
  return this->nodeDescription == ns.nodeDescription && this->nodeId == ns.nodeId &&
         this->released == ns.released && this->sequenceId == ns.sequenceId &&
         this->nodePosition == ns.nodePosition;
}

void NodeState::fromNode(const Node& n) {
  this->nodeId = n.nodeId;
  this->sequenceId = n.sequenceId;
  this->nodeDescription = n.nodeDescription;
  this->nodePosition = n.nodePosition;
  this->released = n.released;
}

}  // namespace impl
}  // namespace vda5050_connector
