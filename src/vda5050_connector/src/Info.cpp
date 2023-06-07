#include "vda5050_connector/Info.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json Info::to_json() {
  Json j = Json{{"infoType", this->infoType}, {"infoDescription", this->infoDescription},
      {"infoLevel", this->infoLevel}};
  Json Info_ref_j = Json::array();
  for (auto& pt : this->infoReferences) {
    Info_ref_j.push_back(pt.to_json());
  }
  j["infoReferences"] = Info_ref_j;
  return j;
}

void Info::from_json(const Json& j) {
  j.at("infoType").get_to(this->infoType);
  j.at("infoDescription").get_to(this->infoDescription);
  j.at("infoLevel").get_to(this->infoLevel);
  for (const auto& j : j.at("infoReferences")) {
    InfoReference i;
    i.from_json(j);
    this->infoReferences.push_back(i);
  }
}

}  // namespace impl
}  // namespace vda5050_connector