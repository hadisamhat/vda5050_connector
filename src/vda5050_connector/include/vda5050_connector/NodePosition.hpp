#pragma once
#include "vda5050_connector_interface/BaseNodePosition.hpp"

namespace vda5050_connector {
namespace impl {
class NodePosition : public interface::BaseNodePosition {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  bool operator==(const NodePosition& np) const;
};
}  // namespace impl
}  // namespace vda5050_connector