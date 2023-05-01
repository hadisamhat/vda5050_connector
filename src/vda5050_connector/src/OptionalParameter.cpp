#include "vda5050_connector/OptionalParameter.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void OptionalParameter::from_json(const nlohmann::json& j) {
  j.at("parameter").get_to(this->parameter);
  j.at("support").get_to(this->support);
  j.at("description").get_to(this->description);
}
Json OptionalParameter::to_json() {
  return Json{{"parameter", this->parameter}, {"support", this->support},
      {"description", this->description}};
}

}  // namespace impl
}  // namespace vda5050_connector