#pragma once

#include "vda5050_connector_interface/BaseInterface.hpp"

namespace iw {
namespace vda5050 {
class Position : public vda5050_connector::interface::BaseInterface {
 public:
  double x{0};
  double y{0};
  double z{0};
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace vda5050
}  // namespace iw
