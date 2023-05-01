#pragma once
#include "vda5050_connector/ControlPoint.hpp"
#include "vda5050_connector_interface/BaseTrajectory.hpp"

namespace vda5050_connector {
namespace impl {
class Trajectory : public interface::BaseTrajectory<ControlPoint> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
