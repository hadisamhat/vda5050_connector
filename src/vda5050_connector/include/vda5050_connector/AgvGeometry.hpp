#pragma once
#include "vda5050_connector/Envelope2d.hpp"
#include "vda5050_connector/Envelope3d.hpp"
#include "vda5050_connector/WheelDefinition.hpp"
#include "vda5050_connector_interface/BaseAgvGeometry.hpp"

namespace vda5050_connector {
namespace impl {
class AgvGeometry : public interface::BaseAgvGeometry<WheelDefinition, Envelope2d, Envelope3d> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector