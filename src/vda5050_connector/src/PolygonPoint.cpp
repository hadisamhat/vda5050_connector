#include "vda5050_connector/PolygonPoint.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void PolygonPoint::from_json(const nlohmann::json& j) {
  j.at("x").get_to(this->x);
  j.at("y").get_to(this->y);
}
Json PolygonPoint::to_json() {
  auto j = Json{{"x", this->x}, {"y", this->y}};
  return j;
}

}  // namespace impl
}  // namespace vda5050_connector