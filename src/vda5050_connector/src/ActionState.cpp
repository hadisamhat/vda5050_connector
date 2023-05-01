#include "vda5050_connector/ActionState.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

bool ActionState::operator==(const ActionState& as) const {
  bool res = this->actionDescription == as.actionDescription;
  res &= this->actionId == as.actionId;
  res &= this->actionStatus == as.actionStatus;
  res &= this->actionType == as.actionType;
  res &= this->resultDescription == as.resultDescription;
  return res;
}

void ActionState::fromAction(const Action& a) {
  this->actionId = a.actionId;
  this->actionType = a.actionType;
  this->actionDescription = a.actionDescription;
  this->actionStatus = interface::ActionStates::WAITING;
}

Json ActionState::to_json() {
  return Json{{"actionId", this->actionId}, {"actionType", this->actionType},
      {"actionDescription", this->actionDescription}, {"actionStatus", this->actionStatus},
      {"resultDescription", this->resultDescription}};
}

void ActionState::from_json(const Json& j) {
  j.at("actionId").get_to(this->actionId);
  j.at("actionType").get_to(this->actionType);
  if (j.find("actionDescription") != j.end()) {
    j.at("actionDescription").get_to(this->actionDescription);
  }
  j.at("actionStatus").get_to(this->actionStatus);
  if (j.find("resultDescription") != j.end()) {
    j.at("resultDescription").get_to(this->resultDescription);
  }
}

}  // namespace impl
}  // namespace vda5050_connector