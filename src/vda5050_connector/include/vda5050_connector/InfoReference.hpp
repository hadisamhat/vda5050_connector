#pragma once
#include "vda5050_connector_interface/BaseInfoReference.hpp"

namespace vda5050_connector {
namespace impl {
class InfoReference : public interface::BaseInfoReference {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  bool operator==(const InfoReference& ir1) const;
};
}  // namespace impl
}  // namespace vda5050_connector
