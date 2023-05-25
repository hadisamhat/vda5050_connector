#include "iw_vda5050_connector/Action.hpp"

using namespace boost::algorithm;
using namespace std;
using namespace vda5050_connector::impl;
using std::nullopt;
using std::optional;
using Json = nlohmann::json;

namespace iw {
namespace vda5050 {

void Action::UpperFirst(string& s) {
  if (!s.empty()) s[0] = toupper(s[0]);
}

Json Action::to_json() {
  UpperFirst(this->actionType);
  Json j = Json{{"actionType", this->actionType}, {"actionId", this->actionId},
      {"actionDescription", this->actionDescription}, {"blockingType", this->actionBlockingType}};
  Json action_params_j = Json::array();
  for (auto& ap : this->actionParameters) {
    action_params_j.push_back(ap.to_json());
  }
  j["actionParameters"] = action_params_j;
  return j;
}
void Action::from_json(const Json& j) {
  this->actionType = to_lower_copy(j.at("actionType").get<string>());
  j.at("actionId").get_to(this->actionId);
  if (j.find("actionDescription") != j.end()) {
    j.at("actionDescription").get_to(this->actionDescription);
  }
  j.at("blockingType").get_to(this->actionBlockingType);
  if (j.find("actionParameters") != j.end()) {
    for (const auto& j : j.at("actionParameters")) {
      ActionParameter ap;
      ap.from_json(j);
      this->actionParameters.push_back(ap);
    }
  }
}

bool Action::operator==(const Action& a) const {
  bool res = this->actionId == a.actionId;
  res &= this->actionType == a.actionType;
  res &= this->actionParameters == a.actionParameters;
  res &= this->actionDescription == a.actionDescription;
  res &= this->actionBlockingType == a.actionBlockingType;
  return res;
}
bool Action::hasValidActionParameters() const {
  // Check that all required action parameters keys are there.
  for (const auto& param : actionParametersPerAction.at(this->actionType)) {
    auto actionParameter = this->getParameterByKey<string>(param.c_str());
    if (!actionParameter) {
      return false;
    }
  }

  // Case by case check for each action since each action has it's own set of action parameters.

  // Dock actions.
  if (this->actionType == strActions::DOCK) {
    // Iterator to get the action parameter based on the key.
    auto actionParameter = this->getParameterByKey<string>(ActionParamConsts::DOCK_TYPE);
    if (!actionParameter) return false;

    // Find the action parameter value in the list of allowed actions.
    auto it2 = find(allowedNodeActions.begin(), allowedNodeActions.end(),
        to_lower_copy(actionParameter.value()));
    if (it2 != allowedNodeActions.end()) {
      return true;
    }
    // TODO : Inform that the docking type is not defined.
    return false;
  }

  // Wait actions.
  if (this->actionType == strActions::WAIT) {
    // Iterator to get the action parameter based on the key.
    auto actionParameter = this->getParameterByKey<int>(ActionParamConsts::WAIT_FOR);
    if (!actionParameter) return false;

    return true;
  }
  // Init position action.
  if (this->actionType == strActions::INIT_POSITION) {
    auto actionParameterX = this->getParameterByKey<double>(ActionParamConsts::X);
    if (!actionParameterX) return false;
    auto actionParameterY = this->getParameterByKey<double>(ActionParamConsts::Y);
    if (!actionParameterY) return false;
    auto actionParameterTheta = this->getParameterByKey<double>(ActionParamConsts::THETA);
    if (!actionParameterTheta) return false;

    return true;
  }
  // Trigger action.
  if (this->actionType == strActions::TRIGGER) {
    auto actionParameterX = this->getParameterByKey<string>(ActionParamConsts::TRIGGER_EVENT);
    if (!actionParameterX) return false;

    return true;
  }
  return false;
}

bool Action::requiresActionParameters() const {
  return actionParametersPerAction.find(this->actionType) != actionParametersPerAction.end();
}
bool Action::isSupportedAction(const vector<std::string>& allowedActions) const {
  // Look for the action in the vector of allowed actions.
  auto it = find(allowedActions.begin(), allowedActions.end(), this->actionType);
  if (it == allowedActions.end()) return false;
  // If the action does not require parameters, return true.
  if (!this->requiresActionParameters()) {
    return true;
  }
  // If the action does require parameters, check the validity of the parameters.
  return this->hasValidActionParameters();
}

}  // namespace vda5050
}  // namespace iw