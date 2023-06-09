#pragma once
#include "iw_vda5050_connector/Action.hpp"
#include "vda5050_connector/ProtocolHeader.hpp"
#include "vda5050_connector_interface/BaseInstantAction.hpp"

namespace iw {
namespace vda5050 {
class InstantAction : public vda5050_connector::interface::BaseInstantAction<Action> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace vda5050
}  // namespace iw
