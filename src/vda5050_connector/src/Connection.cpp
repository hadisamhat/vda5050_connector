#include "vda5050_connector/Connection.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

// ConnectionState
Json Connection::to_json() {
  Json j = Json{{"connectionState", this->connectionState}};
  j.merge_patch(this->header.to_json());
  return j;
}
void Connection::from_json(const Json& j) {
  this->header.from_json(j);
  j.at("connectionState").get_to(this->connectionState);
}
}  // namespace impl
}  // namespace vda5050_connector
