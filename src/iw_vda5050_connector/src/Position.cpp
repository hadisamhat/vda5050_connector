#include "iw_vda5050_connector/Position.hpp"

using namespace std;
using Json = nlohmann::json;
using namespace vda5050_connector::interface;

namespace iw {
namespace vda5050 {

Json Position::to_json() { return Json{{"x", this->x}, {"y", this->y}, {"z", this->z}}; }

void Position::from_json(const Json& j) {
  j.at("x").get_to(this->x);
  j.at("y").get_to(this->y);
  j.at("z").get_to(this->z);
}

}  // namespace vda5050
}  // namespace iw