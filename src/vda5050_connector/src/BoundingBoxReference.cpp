#include "vda5050_connector/BoundingBoxReference.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void BoundingBoxReference::from_json(const nlohmann::json& j) {
  j.at("x").get_to(this->x);
  j.at("y").get_to(this->y);
  j.at("z").get_to(this->z);
  if (j.find("Theta") != j.end() && !j.at("Theta").is_null()) {
    j.at("Theta").get_to(this->theta);
  }
}
Json BoundingBoxReference::to_json() {
  return Json{{"x", this->x}, {"y", this->y}, {"z", this->z}, {"Theta", this->theta}};
}
}  // namespace impl
}  // namespace vda5050_connector