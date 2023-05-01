#include "vda5050_connector/TimingInfo.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void TimingInfo::from_json(const nlohmann::json& j) {
  j.at("minOrderInterval").get_to(this->minOrderInterval);
  j.at("minStateInterval").get_to(this->minStateInterval);
  j.at("defaultStateInterval").get_to(this->defaultStateInterval);
  j.at("visualizationInterval").get_to(this->visualizationInterval);
}
Json TimingInfo::to_json() {
  return Json{{"minOrderInterval", this->minOrderInterval},
      {"minStateInterval", this->minStateInterval},
      {"defaultStateInterval", this->defaultStateInterval},
      {"visualizationInterval", this->visualizationInterval}};
}

}  // namespace impl
}  // namespace vda5050_connector