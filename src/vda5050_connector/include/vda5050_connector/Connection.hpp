#pragma once
#include "vda5050_connector/ProtocolHeader.hpp"
#include "vda5050_connector_interface/BaseConnection.hpp"
namespace vda5050_connector {
namespace impl {
class Connection : public interface::BaseConnection<ProtocolHeader> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector