#pragma once

#include "iw_vda5050_connector/Position.hpp"
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace iw {
namespace vda5050 {
class MetaData : public vda5050_connector::interface::BaseInterface {
 public:
  double resolution;  ///< meters per pixel
  Position origin;
  int widthInPixels;
  int heightInPixels;
  double occupiedThresh;
  double freeThresh;  // OPTIONAL, Unit : meters
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace vda5050
}  // namespace iw
