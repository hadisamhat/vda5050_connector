#include "vda5050_connector/AgvGeometry.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json AgvGeometry::to_json() {
  return Json{{"maxStringLens", this->maxStringLens.to_json()},
      {"maxArrayLens", this->maxArrayLens.to_json()}, {"timing", this->timing.to_json()}};
}

void AgvGeometry::from_json(const Json& j) {
  this->maxStringLens.from_json(j.at("maxStringLens"));
  this->maxArrayLens.from_json(j.at("maxArrayLens"));
  this->timing.from_json(j.at("timing"));
}

}  // namespace impl
}  // namespace vda5050_connector