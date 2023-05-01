#include "vda5050_connector/TypeSpecification.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void TypeSpecification::from_json(const nlohmann::json& j) {
  j.at("seriesName").get_to(this->seriesName);
  j.at("seriesDescription").get_to(this->seriesDescription);
  j.at("agvKinematic").get_to(this->agvKinematic);
  j.at("agvClass").get_to(this->agvClass);
  j.at("maxLoadMass").get_to(this->maxLoadMass);
  j.at("localizationTypes").get_to(this->localizationTypes);
  j.at("navigationTypes").get_to(this->navigationTypes);
}
Json TypeSpecification::to_json() {
  return Json{{"seriesName", this->seriesName}, {"seriesDescription", this->seriesDescription},
      {"agvKinematic", this->agvKinematic}, {"agvClass", this->agvClass},
      {"maxLoadMass", this->maxLoadMass}, {"localizationTypes", this->localizationTypes},
      {"navigationTypes", this->navigationTypes}};
}

}  // namespace impl
}  // namespace vda5050_connector