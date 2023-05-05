#include "vda5050_connector/Velocity.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json Velocity::to_json() {
  return Json{{"vx", this->vx}, {"vy", this->vy}, {"omega", this->omega}};
}

void Velocity::from_json(const Json& j) {
  j.at("vx").get_to(this->vx);
  j.at("vy").get_to(this->vy);
  j.at("omega").get_to(this->omega);
}

}  // namespace impl
}  // namespace vda5050_connector