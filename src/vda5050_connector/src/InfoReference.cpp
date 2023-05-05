#include "vda5050_connector/InfoReference.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json InfoReference::to_json() {
  return {{"referenceKey", this->referenceKey}, {"referenceValue", this->referenceValue}};
}

void InfoReference::from_json(const Json& j) {
  j.at("referenceKey").get_to(this->referenceKey);
  j.at("referenceValue").get_to(this->referenceValue);
}

bool InfoReference::operator==(const InfoReference& ir1) const {
  return this->referenceKey == ir1.referenceKey && this->referenceValue == ir1.referenceValue;
}
}  // namespace impl
}  // namespace vda5050_connector