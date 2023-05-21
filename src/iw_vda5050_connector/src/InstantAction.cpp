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
  j.merge_patch(this->header.to_json());
  return j;
}

void InstantAction::from_json(const Json& j) {
  this->header.from_json(j);
  for (const auto& j : j.at("instantActions")) {
    Action ac;
    ac.from_json(j);
    this->instantActions.push_back(ac);
  }
}

}  // namespace impl
}  // namespace vda5050_connector