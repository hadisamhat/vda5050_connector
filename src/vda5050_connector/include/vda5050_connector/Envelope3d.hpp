#pragma once
#include "vda5050_connector_interface/BaseEnvelope3d.hpp"

namespace vda5050_connector {
namespace impl {
class Envelope3d : public interface::BaseEnvelope3d {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector