#include "vda5050_connector/Error.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json Error::to_json() {
  Json j = Json{{"errorType", this->errorType}, {"errorDescription", this->errorDescription},
      {"errorLevel", this->errorLevel}, {"errorHint", this->errorHint}};
  Json error_ref_j = Json::array();
  for (auto& pt : this->errorReferences) {
    error_ref_j.push_back(pt.to_json());
  }
  j["errorReferences"] = error_ref_j;
  return j;
}

void Error::from_json(const Json& j) {
  j.at("errorType").get_to(this->errorType);
  j.at("errorDescription").get_to(this->errorDescription);
  j.at("errorLevel").get_to(this->errorLevel);
  j.at("errorHint").get_to(this->errorHint);
  for (const auto& j : j.at("errorReferences")) {
    ErrorReference e;
    e.from_json(j);
    this->errorReferences.push_back(e);
  }
}

bool Error::operator==(const Error& e) const {
  bool res = this->errorType == e.errorType;
  res &= this->errorDescription == e.errorDescription;
  res &= this->errorLevel == e.errorLevel;
  res &= this->errorReferences == e.errorReferences;
  res &= this->errorHint == e.errorHint;
  return res;
}

}  // namespace impl
}  // namespace vda5050_connector