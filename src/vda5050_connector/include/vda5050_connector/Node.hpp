#pragma once
#include "vda5050_connector/Action.hpp"
#include "vda5050_connector/NodePosition.hpp"
#include "vda5050_connector_interface/BaseNode.hpp"

namespace vda5050_connector {
namespace impl {
class Node : public interface::BaseNode<NodePosition, Action> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  std::optional<Action> tryGetActionOfType(const std::string& action_type) override;
  bool hasActionOfType(const std::string& action_type) const override;
  std::optional<std::string> getTypeFromDescription() const override;
  bool operator==(const Node& n) const;
  void fromNode(const Node& node);
};
}  // namespace impl
}  // namespace vda5050_connector
