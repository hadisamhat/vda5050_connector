#pragma once
#include "vda5050_connector/ArrayLength.hpp"
#include "vda5050_connector/StringLength.hpp"
#include "vda5050_connector/TimingInfo.hpp"
#include "vda5050_connector_interface/BaseProtocolLimits.hpp"

namespace vda5050_connector {
namespace impl {
class ProtocolLimits : public interface::BaseProtocolLimits<StringLength, TimingInfo, ArrayLength> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
