#include "iw_vda5050_connector/Zone.hpp"

using Json = nlohmann::json;
using namespace boost::algorithm;
using namespace std;
using namespace vda5050_connector::impl;

namespace iw {
namespace vda5050 {

Json Zone::to_json() {
  Json j = Json{{"zoneId", this->zoneId}, {"zoneType", this->zoneType}, {"mapId", this->mapId},
      {"zoneName", this->zoneName}, {"zoneDescription", this->zoneDescription}};
  Json zone_params_j = Json::array();
  for (auto& zp : this->zoneParameters) {
    zone_params_j.push_back(zp.to_json());
  }
  j["polygon"] = zone_params_j;
  Json polygon_j = Json::array();
  for (auto& p : this->polygon) {
    polygon_j.push_back(p.to_json());
  }
  j["polygon"] = polygon_j;
  return j;
}
void Zone::from_json(const Json& j) {
  this->zoneType = to_lower_copy(j.at("zoneType").get<string>());
  j.at("zoneId").get_to(this->zoneId);
  j.at("mapId").get_to(this->mapId);
  if (j.find("zoneName") != j.end() && !j.at("zoneName").is_null()) {
    j.at("zoneName").get_to(this->zoneName);
  }
  if (j.find("zoneDescription") != j.end() && !j.at("zoneDescription").is_null()) {
    j.at("zoneDescription").get_to(this->zoneDescription);
  }
  for (const auto& j : j.at("zoneParameters")) {
    ZoneParameter zp;
    zp.from_json(j);
    this->zoneParameters.push_back(zp);
  }
  for (const auto& j : j.at("polygon")) {
    PolygonPoint pp;
    pp.from_json(j);
    this->polygon.push_back(pp);
  }
}
}  // namespace vda5050
}  // namespace iw