#include "iw_vda5050_connector/Load.hpp"

using Json = nlohmann::json;

namespace iw {
namespace vda5050 {
void Load::from_json(const nlohmann::json& j) {
  j.at("loadId").get_to(this->loadId);
  j.at("loadType").get_to(this->loadType);
  j.at("loadPosition").get_to(this->loadPosition);
  this->bbReference.from_json(j.at("boundingBoxReference"));
  this->dimensions.from_json(j.at("loadDimensions"));
}
Json Load::to_json() {
  return Json{{"loadId", this->loadId}, {"loadType", this->loadType},
      {"loadPosition", this->loadPosition}, {"boundingBoxReference", this->bbReference.to_json()},
      {"loadDimensions", this->dimensions.to_json()}};
}

}  // namespace vda5050
}  // namespace iw