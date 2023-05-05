#pragma once
#include "vda5050_connector/InfoReference.hpp"
#include "vda5050_connector_interface/BaseInfo.hpp"

namespace vda5050_connector {
namespace impl {
class Info : public interface::BaseInfo<InfoReference> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector
