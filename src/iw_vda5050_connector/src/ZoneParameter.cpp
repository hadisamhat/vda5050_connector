#include "iw_vda5050_connector/ZoneParameter.hpp"

using namespace boost::algorithm;
using namespace std;
using std::experimental::nullopt;
using std::experimental::optional;
using Json = nlohmann::json;

namespace iw {
namespace vda5050 {

string ZoneParameter::UpperFirst(string s) {
  if (!s.empty()) s[0] = toupper(s[0]);
  return s;
}


Json ZoneParameter::to_json() {
  return Json{{"key", UpperFirst(this->key)}, {"value", this->value}};
}
void ZoneParameter::from_json(const Json& j) {
  this->key = to_lower_copy(j.at("key").get<string>());
  this->value = j.at("value");
}

}  // namespace vda5050_connector
}  // namespace iw