#include "vda5050_connector/ActionParameter.hpp"

using namespace boost::algorithm;
using namespace std;
using std::nullopt;
using std::optional;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

void ActionParameter::UpperFirst(string& s) {
  if (!s.empty()) s[0] = toupper(s[0]);
}

bool ActionParameter::operator==(const ActionParameter& ap) const {
  bool res = this->key == ap.key;
  res &= this->value == ap.value;
  return res;
}
Json ActionParameter::to_json() {
  UpperFirst(this->key);
  return Json{{"key", this->key}, {"value", this->value}};
}
void ActionParameter::from_json(const Json& j) {
  this->key = to_lower_copy(j.at("key").get<string>());
  this->value = j.at("value");
}

}  // namespace impl
}  // namespace vda5050_connector