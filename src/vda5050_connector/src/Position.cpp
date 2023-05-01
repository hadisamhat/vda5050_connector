#include "vda5050_connector/Position.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json Position::to_json() { return Json{{"x", this->x}, {"y", this->y}, {"theta", this->theta}}; }

void Position::from_json(const Json& j) {
  j.at("x").get_to(this->x);
  j.at("y").get_to(this->y);
  j.at("theta").get_to(this->theta);
}

}  // namespace impl
}  // namespace vda5050_connector