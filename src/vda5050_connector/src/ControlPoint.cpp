#include "vda5050_connector/ControlPoint.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void ControlPoint::from_json(const nlohmann::json& j) {
  j.at("x").get_to(this->x);
  j.at("y").get_to(this->y);
  if (j.find("orientation") != j.end() && !j.at("orientation").is_null()) {
    j.at("orientation").get_to(this->orientation);
  }
  j.at("weight").get_to(this->weight);
}
Json ControlPoint::to_json() {
  return Json{
      {"x", this->x}, {"y", this->y}, {"orientation", this->orientation}, {"weight", this->weight}};
}

}  // namespace impl
}  // namespace vda5050_connector