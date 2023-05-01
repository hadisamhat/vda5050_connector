#include "vda5050_connector/LoadDimension.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json LoadDimension::to_json() {
  return Json{{"length", this->length}, {"width", this->width}, {"height", this->height}};
}

void LoadDimension::from_json(const Json& j) {
  j.at("length").get_to(this->length);
  j.at("width").get_to(this->width);
  j.at("height").get_to(this->height);
}

}  // namespace impl
}  // namespace vda5050_connector