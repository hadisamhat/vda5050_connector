#include "vda5050_connector/LoadSet.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void LoadSet::from_json(const nlohmann::json& j) {
  j.at("setName").get_to(this->setName);
  j.at("loadType").get_to(this->loadType);
  j.at("description").get_to(this->description);
  j.at("loadPositions").get_to(this->loadPositions);
  j.at("maxWeight").get_to(this->maxWeight);
  j.at("minLoadhandlingHeight").get_to(this->minLoadhandlingHeight);
  j.at("maxLoadhandlingHeight").get_to(this->minLoadhandlingDepth);
  j.at("minLoadhandlingDepth").get_to(this->maxLoadhandlingDepth);
  j.at("maxLoadhandlingDepth").get_to(this->maxLoadhandlingDepth);
  j.at("minLoadhandlingTilt").get_to(this->minLoadhandlingTilt);
  j.at("maxLoadhandlingTilt").get_to(this->maxLoadhandlingTilt);
  j.at("agvSpeedLimit").get_to(this->agvSpeedLimit);
  j.at("agvDecelerationLimit").get_to(this->agvDecelerationLimit);
  j.at("agvAccelerationLimit").get_to(this->agvAccelerationLimit);
  j.at("pickTime").get_to(this->pickTime);
  j.at("dropTime").get_to(this->dropTime);
  this->boundingBoxReference.from_json(j.at("boundingBoxReference"));
  this->loadDimensions.from_json(j.at("loadDimensions"));
}
Json LoadSet::to_json() {
  return Json{{"setName", this->setName}, {"loadType", this->loadType},
      {"description", this->description}, {"loadPositions", this->loadPositions},
      {"maxWeight", this->maxWeight}, {"minLoadhandlingHeight", this->minLoadhandlingHeight},
      {"maxLoadhandlingHeight", this->maxLoadhandlingHeight},
      {"minLoadhandlingDepth", this->minLoadhandlingDepth},
      {"maxLoadhandlingDepth", this->maxLoadhandlingDepth},
      {"minLoadhandlingTilt", this->minLoadhandlingTilt},
      {"maxLoadhandlingTilt", this->maxLoadhandlingTilt}, {"agvSpeedLimit", this->agvSpeedLimit},
      {"agvDecelerationLimit", this->agvDecelerationLimit},
      {"agvAccelerationLimit", this->agvAccelerationLimit}, {"pickTime", this->pickTime},
      {"dropTime", this->dropTime}, {"boundingBoxReference", this->boundingBoxReference.to_json()},
      {"loadDimensions", this->loadDimensions.to_json()}};
}

}  // namespace impl
}  // namespace vda5050_connector