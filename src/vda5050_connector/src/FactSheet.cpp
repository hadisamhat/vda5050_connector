#include "vda5050_connector/FactSheet.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
Json FactSheet::to_json() {
  Json j = Json{{"typeSpecification", this->typeSpecification.to_json()},
      {"physicalParameters", this->physicalParameters.to_json()},
      {"protocolLimits", this->protocolLimits.to_json()},
      {"protocolFeatures", this->protocolFeatures.to_json()},
      {"agvGeometry", this->agvGeometry.to_json()},
      {"loadSpecification", this->loadSpecification.to_json()}};
  j.merge_patch(this->header.to_json());
  return j;
}

void FactSheet::from_json(const Json& j) {
  this->header.from_json(j);
  this->typeSpecification.from_json(j.at("typeSpecification"));
  this->physicalParameters.from_json(j.at("physicalParameters"));
  this->protocolLimits.from_json(j.at("protocolLimits"));
  this->protocolFeatures.from_json(j.at("protocolFeatures"));
  this->agvGeometry.from_json(j.at("agvGeometry"));
  this->loadSpecification.from_json(j.at("loadSpecification"));
}
}  // namespace impl
}  // namespace vda5050_connector