#include "vda5050_connector/SafetyState.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json SafetyState::to_json() {
  return Json{{"eStop", this->eStop}, {"fieldViolation", this->fieldViolation}};
}

void SafetyState::from_json(const Json& j) {
  j.at("eStop").get_to(this->eStop);
  j.at("fieldViolation").get_to(this->fieldViolation);
}

}  // namespace impl
}  // namespace vda5050_connector