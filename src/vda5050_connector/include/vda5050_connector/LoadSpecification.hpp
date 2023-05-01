#pragma once
#include "vda5050_connector/LoadSet.hpp"
#include "vda5050_connector_interface/BaseLoadSpecification.hpp"

namespace vda5050_connector {
namespace impl {
class LoadSpecification : public interface::BaseLoadSpecification<LoadSet> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
