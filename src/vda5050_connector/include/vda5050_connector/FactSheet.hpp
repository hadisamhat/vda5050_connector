#pragma once
#include "vda5050_connector/AgvGeometry.hpp"
#include "vda5050_connector/LoadSpecification.hpp"
#include "vda5050_connector/PhysicalParameters.hpp"
#include "vda5050_connector/ProtocolFeatures.hpp"
#include "vda5050_connector/ProtocolLimits.hpp"
#include "vda5050_connector/TypeSpecification.hpp"
#include "vda5050_connector_interface/BaseFactSheet.hpp"

namespace vda5050_connector {
namespace impl {
class FactSheet : public interface::BaseFactSheet<TypeSpecification, PhysicalParameters,
                      ProtocolLimits, ProtocolFeatures, AgvGeometry, LoadSpecification> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector