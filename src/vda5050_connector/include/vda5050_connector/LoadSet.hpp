#pragma once
#include "vda5050_connector/BoundingBoxReference.hpp"
#include "vda5050_connector/LoadDimension.hpp"
#include "vda5050_connector_interface/BaseLoadSet.hpp"

namespace vda5050_connector {
namespace impl {
class LoadSet : public interface::BaseLoadSet<BoundingBoxReference, LoadDimension> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
