#pragma once
#include "vda5050_connector/ErrorReference.hpp"
#include "vda5050_connector_interface/BaseError.hpp"

namespace vda5050_connector {
namespace impl {
class Error : public interface::BaseError<ErrorReference> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  bool operator==(const Error& er1) const;
};
}  // namespace impl
}  // namespace vda5050_connector
