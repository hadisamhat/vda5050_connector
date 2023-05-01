#pragma once
#include "vda5050_connector_interface/BaseTypeSpecification.hpp"

namespace vda5050_connector {
namespace impl {
class TypeSpecification : public interface::BaseTypeSpecification {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
