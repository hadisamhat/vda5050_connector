#include "vda5050_connector/Envelope3d.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json Envelope3d::to_json() {
  return Json{{"set", this->set}, {"format", this->format}, {"data", this->data},
      {"url", this->url}, {"description", this->description}};
}

void Envelope3d::from_json(const Json& j) {
  j.at("set").get_to(this->set);
  j.at("format").get_to(this->format);
  this->data = j.at("data");
  j.at("url").get_to(this->url);
  j.at("description").get_to(this->description);
}
}  // namespace impl
}  // namespace vda5050_connector