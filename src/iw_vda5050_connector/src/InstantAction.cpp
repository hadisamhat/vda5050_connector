#include "iw_vda5050_connector/InstantAction.hpp"

using namespace std;
using Json = nlohmann::json;
using namespace vda5050_connector::interface;

namespace iw {
namespace vda5050 {

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

}  // namespace vda5050
}  // namespace iw