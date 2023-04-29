#pragma once
#include <experimental/optional>

#include "vda5050_connector_interface/BaseProtocolHeader.hpp"

namespace vda5050_connector {
namespace impl {

class ProtocolHeader : public interface::BaseProtocolHeader {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace impl
}  // namespace vda5050_connector