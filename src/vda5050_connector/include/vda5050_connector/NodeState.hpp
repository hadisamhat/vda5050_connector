#pragma once
#include "vda5050_connector/Node.hpp"
#include "vda5050_connector/NodePosition.hpp"
#include "vda5050_connector_interface/BaseNodeState.hpp"

namespace vda5050_connector {
namespace impl {
class NodeState : public interface::BaseNodeState<Node, NodePosition> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  void fromNode(const Node& node) override;
  bool operator==(const NodeState& ns1) const;
};
}  // namespace impl
}  // namespace vda5050_connector
