#pragma once
#include "vda5050_connector/Position.hpp"
#include "vda5050_connector_interface/BaseWheelDefinition.hpp"

namespace vda5050_connector {
namespace impl {
class WheelDefinition : public interface::BaseWheelDefinition<Position> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector