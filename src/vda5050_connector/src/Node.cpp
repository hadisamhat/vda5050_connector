#include "vda5050_connector/Node.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void Node::from_json(const nlohmann::json& j) {
  j.at("nodeId").get_to(this->nodeId);
  j.at("sequenceId").get_to(this->sequenceId);
  if (j.find("nodeDescription") != j.end() && !j.at("nodeDescription").is_null()) {
    j.at("nodeDescription").get_to(this->nodeDescription);
  }
  j.at("released").get_to(this->released);
  this->nodePosition.from_json(j.at("nodePosition"));
  for (const auto& j : j.at("actions")) {
    Action a;
    a.from_json(j);
    this->actions.push_back(a);
  }
}
Json Node::to_json() {
  Json j = Json{{"nodeId", this->nodeId}, {"sequenceId", this->sequenceId},
      {"nodeDescription", this->nodeDescription}, {"released", this->released},
      {"nodePosition", this->nodePosition.to_json()}};
  Json actions_j = Json::array();
  for (auto& ls : this->actions) {
    actions_j.push_back(ls.to_json());
  }
  j["actions"] = actions_j;
  return j;
}

std::experimental::optional<std::string> Node::getTypeFromDescription() const {
  std::string typestring = "Type:";
  auto type_key_idx = nodeDescription.find(typestring);
  if (type_key_idx == std::string::npos) return std::experimental::nullopt;
  if ((type_key_idx + typestring.size()) >= nodeDescription.size())
    return std::experimental::nullopt;
  constexpr char delim = ' ';
  std::stringstream ss;
  ss << this->nodeDescription.substr(type_key_idx + typestring.size(),
      nodeDescription.size() - type_key_idx - typestring.size() + 1);
  std::string section;
  std::getline(ss, section, delim);
  if (!std::getline(ss, section, delim)) return std::experimental::nullopt;
  return section;
}

/**
 * @brief: returns the first occurence of an action of the type: action_type and std::nullopt if no
 * action of this type is found
 * @param: action_type
 * @return: std::optional<Action>
 * */
std::experimental::optional<Action> Node::tryGetActionOfType(const std::string& action_type) {
  auto it = find_if(actions.begin(), actions.end(),
      [&](const Action& a) { return a.actionType == action_type; });
  if (it == actions.end()) return std::experimental::nullopt;
  return *it;
}

bool Node::hasActionOfType(const std::string& action_type) const {
  auto it = find_if(actions.begin(), actions.end(),
      [&](const Action& a) { return a.actionType == action_type; });
  return it != actions.end();
}

bool Node::operator==(const Node& n) const {
  bool res = this->nodeId == n.nodeId;
  res &= this->nodePosition == n.nodePosition;
  res &= this->nodeDescription == n.nodeDescription;
  res &= this->sequenceId == n.sequenceId;
  res &= this->released == n.released;
  res &= this->actions == n.actions;
  return res;
}

}  // namespace impl
}  // namespace vda5050_connector