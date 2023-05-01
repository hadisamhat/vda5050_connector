#pragma once
#include "vda5050_connector_interface/BaseStringLength.hpp"

namespace vda5050_connector {
namespace impl {
class StringLength : public interface::BaseStringLength {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
