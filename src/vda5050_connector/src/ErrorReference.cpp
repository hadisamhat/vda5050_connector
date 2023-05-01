#include "vda5050_connector/ErrorReference.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json ErrorReference::to_json() {
  return {{"referenceKey", this->referenceKey}, {"referenceValue", this->referenceValue}};
}

void ErrorReference::from_json(const Json& j) {
  j.at("referenceKey").get_to(this->referenceKey);
  j.at("referenceValue").get_to(this->referenceValue);
}

bool ErrorReference::operator==(const ErrorReference& er) const {
  return this->referenceKey == er.referenceKey && this->referenceValue == er.referenceValue;
}
}  // namespace impl
}  // namespace vda5050_connector