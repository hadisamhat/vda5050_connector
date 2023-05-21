#include "iw_vda5050_connector/InteractionZoneState.hpp"

using Json = nlohmann::json;

namespace iw {
namespace vda5050 {
void InteractionZoneState::from_json(const nlohmann::json& j) {
  j.at("zoneId").get_to(this->zoneId);
  j.at("status").get_to(this->status);
}
Json InteractionZoneState::to_json() {
  return Json{{"zoneId", this->zoneId}, {"status", this->status}};
}
}  // namespace vda5050_connector

}  // namespace iw