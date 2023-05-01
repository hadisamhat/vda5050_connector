#include "vda5050_connector/ProtocolFeatures.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void ProtocolFeatures::from_json(const nlohmann::json& j) {
  j.at("resultDescription").get_to(this->resultDescription);
  for (const auto& j : j.at("agvActions")) {
    AgvAction aa;
    aa.from_json(j);
    this->agvActions.push_back(aa);
  }
  for (const auto& j : j.at("optionalParameters")) {
    OptionalParameter op;
    op.from_json(j);
    this->optionalParameters.push_back(op);
  }
}
Json ProtocolFeatures::to_json() {
  Json j = Json{{"resultDescription", this->resultDescription}};
  Json agv_action_j = Json::array();
  for (auto& aa : this->agvActions) {
    agv_action_j.push_back(aa.to_json());
  }
  j["agvActions"] = agv_action_j;
  Json optional_param_j = Json::array();
  for (auto& op : this->optionalParameters) {
    optional_param_j.push_back(op.to_json());
  }
  j["optionalParameters"] = optional_param_j;
  return j;
}

}  // namespace impl
}  // namespace vda5050_connector