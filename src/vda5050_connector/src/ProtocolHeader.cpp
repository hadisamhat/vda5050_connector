#include "vda5050_connector/ProtocolHeader.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void ProtocolHeader::from_json(const nlohmann::json& j) {
  j.at("headerId").get_to(this->headerId);
  j.at("timeStamp").get_to(this->timestamp);
  j.at("version").get_to(this->version);
  if (!j.at("manufacturer").is_null()) {
    j.at("manufacturer").get_to(this->manufacturer);
  }
  if (!j.at("serialNumber").is_null()) {
    j.at("serialNumber").get_to(this->serialNumber);
  }
}
Json ProtocolHeader::to_json() {
  return Json{{"headerId", this->headerId}, {"timeStamp", this->timestamp},
      {"version", this->version}, {"manufacturer", this->manufacturer},
      {"serialNumber", this->serialNumber}};
}

}  // namespace impl
}  // namespace vda5050_connector