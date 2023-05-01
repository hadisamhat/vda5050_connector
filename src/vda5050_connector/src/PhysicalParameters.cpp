#include "vda5050_connector/PhysicalParameters.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void PhysicalParameters::from_json(const nlohmann::json& j) {
  j.at("speedMin").get_to(this->speedMin);
  j.at("speedMax").get_to(this->speedMax);
  j.at("accelerationMax").get_to(this->accelerationMax);
  j.at("decelerationMax").get_to(this->decelerationMax);
  j.at("heightMin").get_to(this->heightMin);
  j.at("heightMax").get_to(this->heightMax);
  j.at("width").get_to(this->width);
  j.at("length").get_to(this->length);
}
Json PhysicalParameters::to_json() {
  return Json{{"speedMin", this->speedMin}, {"speedMax", this->speedMax},
      {"accelerationMax", this->accelerationMax}, {"decelerationMax", this->decelerationMax},
      {"heightMin", this->heightMin}, {"heightMax", this->heightMax}, {"width", this->width},
      {"length", this->length}};
}

}  // namespace impl
}  // namespace vda5050_connector