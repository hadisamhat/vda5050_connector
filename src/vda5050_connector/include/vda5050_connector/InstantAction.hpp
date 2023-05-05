#pragma once
#include "vda5050_connector/Action.hpp"
#include "vda5050_connector/ProtocolHeader.hpp"
#include "vda5050_connector_interface/BaseInstantAction.hpp"

namespace vda5050_connector {
namespace impl {
class InstantAction : public interface::BaseInstantAction<ProtocolHeader, Action> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
