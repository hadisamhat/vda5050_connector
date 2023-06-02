#include "iw_vda5050_connector/MetaData.hpp"

using namespace std;
using Json = nlohmann::json;
using namespace vda5050_connector::interface;

namespace iw {
namespace vda5050 {

Json MetaData::to_json() {
  return Json{{"resolution", this->resolution}, {"origin", this->origin.to_json()},
      {"widthInPixels", this->widthInPixels}, {"heightInPixels", this->heightInPixels},
      {"occupiedThresh", this->occupiedThresh}, {"freeThresh", this->freeThresh}};
}

void MetaData::from_json(const Json& j) {
  j.at("resolution").get_to(this->resolution);
  this->origin.from_json(j.at("origin"));
  j.at("widthInPixels").get_to(this->widthInPixels);
  j.at("heightInPixels").get_to(this->heightInPixels);
  // TODO: temporary fix until these values are initialized in message coming from cloud
  this->occupiedThresh =
      j.at("occupiedThresh") != 0.0 ? j.at("occupiedThresh").get<double>() : 0.65;
  this->freeThresh = j.at("freeThresh") != 0.0 ? j.at("freeThresh").get<double>() : 0.196;
}

}  // namespace vda5050
}  // namespace iw