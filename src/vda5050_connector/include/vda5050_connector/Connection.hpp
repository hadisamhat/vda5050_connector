#pragma once
#include "vda5050_connector_interface/BaseConnection.hpp"

namespace vda5050_connector {
namespace impl {
class Connection : public interface::BaseConnection {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector