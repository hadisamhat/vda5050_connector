#include "vda5050_connector/InstantAction.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json InstantAction::to_json() {
  Json j;
  Json action_j = Json::array();
  for (auto& pt : this->instantActions) {
    action_j.push_back(pt.to_json());
  }
  j["instantActions"] = action_j;
  return j;
}

void InstantAction::from_json(const Json& j) {
  for (const auto& j : j.at("instantActions")) {
    Action ac;
    ac.from_json(j);
    this->instantActions.push_back(ac);
  }
}

}  // namespace impl
}  // namespace vda5050_connector