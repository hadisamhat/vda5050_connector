#pragma once

#include "iw_vda5050_connector/Zone.hpp"

namespace iw {
namespace vda5050 {
class ZoneUpdate : public vda5050_connector::interface::BaseInterface {
 public:
  std::string zoneSetId;
  std::vector<Zone> zones;
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace vda5050
}  // namespace iw
