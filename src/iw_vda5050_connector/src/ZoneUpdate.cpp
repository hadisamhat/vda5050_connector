#include "iw_vda5050_connector/ZoneUpdate.hpp"

using namespace std;
using Json = nlohmann::json;
using namespace vda5050_connector::interface;

namespace iw {
namespace vda5050 {

Json ZoneUpdate::to_json() {
  auto j = Json{{"zoneSetId", this->zoneSetId}};
  Json zones_j = Json::array();

  for (auto& z : this->zones) {
    zones_j.push_back(z.to_json());
  }
  j["zones"] = zones_j;
  return j;
}

void ZoneUpdate::from_json(const Json& j) {
  j.at("zoneSetId").get_to(this->zoneSetId);
  for (const auto& j : j.at("zones")) {
    Zone z;
    z.from_json(j);
    this->zones.push_back(z);
  }
}

}  // namespace vda5050
}  // namespace iw