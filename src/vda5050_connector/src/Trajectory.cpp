#include "vda5050_connector/Trajectory.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void Trajectory::from_json(const nlohmann::json& j) {
  j.at("degree").get_to(this->degree);
  j.at("knotVector").get_to(this->knotVector);
  for (const auto& j : j.at("controlPoints")) {
    ControlPoint cp;
    cp.from_json(j);
    this->controlPoints.push_back(cp);
  }
}
Json Trajectory::to_json() {
  Json j = Json{{"degree", this->degree}, {"knotVector", this->knotVector}};
  Json control_points_j = Json::array();
  for (auto& cp : this->controlPoints) {
    control_points_j.push_back(cp.to_json());
  }
  j["controlPoints"] = control_points_j;
  return j;
}

}  // namespace impl
}  // namespace vda5050_connector