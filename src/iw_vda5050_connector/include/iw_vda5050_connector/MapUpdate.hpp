#pragma once

#include "vda5050_connector/ProtocolHeader.hpp"
#include "vda5050_connector_interface/BaseTopicInterface.hpp"

namespace iw {
namespace vda5050 {
class MapUpdate : public vda5050_connector::interface::BaseTopicInterface<
                      vda5050_connector::impl::ProtocolHeader> {
 public:
  std::string mapId;
  std::string mapType;
  std::string mapCompression;
  std::string mapDownloadLink;
  std::string mapDownloadProtocol;
  std::string mapCreationDate;  // OPTIONAL
  std::string mapName;          // OPTIONAL
  std::string mapDescription;   // OPTIONAL
  float mapReferenceHeight;     // OPTIONAL, Unit : meters
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace vda5050
}  // namespace iw
