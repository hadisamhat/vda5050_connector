#pragma once
#include "vda5050_connector_interface/BaseAgvPosition.hpp"

namespace vda5050_connector {
namespace impl {
class AgvPosition : public interface::BaseAgvPosition {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector