#include "vda5050_connector/AgvPosition.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void AgvPosition::from_json(const nlohmann::json& j) {
  j.at("positionInitialized").get_to(this->positionInitialized);
  if (j.find("localizationScore") != j.end() && !j.at("localizationScore").is_null()) {
    j.at("localizationScore").get_to(this->localizationScore);
  }
  if (j.find("deviationRange") != j.end() && !j.at("deviationRange").is_null()) {
    j.at("deviationRange").get_to(this->deviationRange);
  }
  j.at("x").get_to(this->x);
  j.at("y").get_to(this->y);
  j.at("theta").get_to(this->theta);
  j.at("mapId").get_to(this->mapId);
  if (j.find("mapDescription") != j.end() && !j.at("mapDescription").is_null()) {
    j.at("mapDescription").get_to(this->mapDescription);
  }
}
Json AgvPosition::to_json() {
  return Json{{"positionInitialized", this->positionInitialized},
      {"localizationScore", this->localizationScore}, {"deviationRange", this->deviationRange},
      {"x", this->x}, {"y", this->y}, {"theta", this->theta}, {"mapId", this->mapId},
      {"mapDescription", this->mapDescription}};
}

}  // namespace impl
}  // namespace vda5050_connector