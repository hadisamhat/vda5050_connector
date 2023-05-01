#pragma once
#include "vda5050_connector/PolygonPoint.hpp"
#include "vda5050_connector_interface/BaseEnvelope2d.hpp"

namespace vda5050_connector {
namespace impl {
class Envelope2d : public interface::BaseEnvelope2d<PolygonPoint> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector