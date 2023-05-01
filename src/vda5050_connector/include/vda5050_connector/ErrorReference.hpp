#pragma once
#include "vda5050_connector_interface/BaseErrorReference.hpp"

namespace vda5050_connector {
namespace impl {
class ErrorReference : public interface::BaseErrorReference {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  bool operator==(const ErrorReference& er1) const;
};
}  // namespace impl
}  // namespace vda5050_connector
