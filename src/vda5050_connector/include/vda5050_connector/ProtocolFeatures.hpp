#pragma once
#include "vda5050_connector/AgvAction.hpp"
#include "vda5050_connector/OptionalParameter.hpp"
#include "vda5050_connector_interface/BaseProtocolFeatures.hpp"

namespace vda5050_connector {
namespace impl {
class ProtocolFeatures : public interface::BaseProtocolFeatures<OptionalParameter, AgvAction> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
