#include "vda5050_connector/Connection.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

// ConnectionState
Json Connection::to_json() {
  return Json{{"connectionState", this->connectionState}};
}
void Connection::from_json(const Json& j) {
  j.at("connectionState").get_to(this->connectionState);
}
}  // namespace impl
}  // namespace vda5050_connector
