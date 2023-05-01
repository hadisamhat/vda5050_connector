#include "vda5050_connector/WheelDefinition.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json WheelDefinition::to_json() {
  return Json{{"type", this->type}, {"isActiveDriven", this->isActiveDriven},
      {"isActiveSteered", this->isActiveSteered}, {"position", this->position.to_json()},
      {"diameter", this->diameter}, {"width", this->width},
      {"centerDisplacement", this->centerDisplacement}, {"constraints", this->constraints}};
}

void WheelDefinition::from_json(const Json& j) {
  j.at("type").get_to(this->type);
  j.at("isActiveDriven").get_to(this->isActiveDriven);
  j.at("isActiveSteered").get_to(this->isActiveSteered);
  this->position.from_json(j.at("position"));
  j.at("diameter").get_to(this->diameter);
  j.at("width").get_to(this->width);
  j.at("centerDisplacement").get_to(this->centerDisplacement);
  j.at("constraints").get_to(this->constraints);
}

}  // namespace impl
}  // namespace vda5050_connector