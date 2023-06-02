#include "iw_vda5050_connector/MapUpdate.hpp"

using namespace std;
using Json = nlohmann::json;
using namespace vda5050_connector::interface;

namespace iw {
namespace vda5050 {

Json MapUpdate::to_json() {
  auto j = Json{{"mapId", this->mapId}, {"mapType", this->mapType},
      {"mapCompression", this->mapCompression}, {"mapDownloadLink", this->mapDownloadLink},
      {"mapDownloadProtocol", this->mapDownloadProtocol},
      {"mapCreationDate", this->mapCreationDate}, {"mapName", this->mapName},
      {"mapDescription", this->mapDescription}, {"mapReferenceHeight", this->mapReferenceHeight},
      {"mapMetaData", this->metaData.to_json()}};

  j.merge_patch(this->header.to_json());
  return j;
}

void MapUpdate::from_json(const Json& j) {
  this->header.from_json(j);
  j.at("mapId").get_to(this->mapId);
  j.at("mapType").get_to(this->mapType);
  j.at("mapCompression").get_to(this->mapCompression);
  j.at("mapDownloadLink").get_to(this->mapDownloadLink);
  j.at("mapDownloadProtocol").get_to(this->mapDownloadProtocol);
  this->metaData.from_json(j.at("mapMetaData"));
  if (!j.at("mapCreationDate").is_null()) {
    j.at("mapCreationDate").get_to(this->mapCreationDate);
  }
  if (!j.at("mapName").is_null()) {
    j.at("mapName").get_to(this->mapName);
  }
  if (!j.at("mapDescription").is_null()) {
    j.at("mapDescription").get_to(this->mapDescription);
  }
  if (!j.at("mapReferenceHeight").is_null()) {
    j.at("mapReferenceHeight").get_to(this->mapReferenceHeight);
  }
}

}  // namespace vda5050
}  // namespace iw