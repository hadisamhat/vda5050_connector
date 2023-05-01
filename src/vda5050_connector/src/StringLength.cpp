#include "vda5050_connector/StringLength.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void StringLength::from_json(const nlohmann::json& j) {
  j.at("msgLen").get_to(this->msgLen);
  j.at("topicSerialLen").get_to(this->topicSerialLen);
  j.at("topicElemLen").get_to(this->topicElemLen);
  j.at("idLen").get_to(this->idLen);
  j.at("idNumericalOnly").get_to(this->idNumericalOnly);
  j.at("enumLen").get_to(this->enumLen);
  j.at("loadIdLen").get_to(this->loadIdLen);
}
Json StringLength::to_json() {
  return Json{{"msgLen", this->msgLen}, {"topicSerialLen", this->topicSerialLen},
      {"topicElemLen", this->topicElemLen}, {"idLen", this->idLen},
      {"idNumericalOnly", this->idNumericalOnly}, {"enumLen", this->enumLen},
      {"loadIdLen", this->loadIdLen}};
}

}  // namespace impl
}  // namespace vda5050_connector