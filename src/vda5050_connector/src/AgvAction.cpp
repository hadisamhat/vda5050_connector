#include "vda5050_connector/AgvAction.hpp"

using namespace boost::algorithm;
using namespace std;
using std::experimental::nullopt;
using std::experimental::optional;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

string AgvAction::UpperFirst(string s) {
  if (!s.empty()) s[0] = toupper(s[0]);
  return s;
}

Json AgvAction::to_json() {
  Json j = Json{{"actionType", UpperFirst(this->actionType)},
      {"actionDescription", this->actionDescription}, {"actionScope", this->actionScope}};
  Json action_params_j = Json::array();
  for (auto& ap : this->actionParameters) {
    action_params_j.push_back(ap.to_json());
  }
  j["actionParameters"] = action_params_j;
  return j;
}
void AgvAction::from_json(const Json& j) {
  this->actionType = to_lower_copy(j.at("actionType").get<string>());
  if (j.find("actionDescription") != j.end()) {
    j.at("actionDescription").get_to(this->actionDescription);
  }
  j.at("actionScope").get_to(this->actionScope);
  if (j.find("actionParameters") != j.end()) {
    for (const auto& j : j.at("actionParameters")) {
      ActionParameter ap;
      ap.from_json(j);
      this->actionParameters.push_back(ap);
    }
  }
}
}  // namespace impl
}  // namespace vda5050_connector