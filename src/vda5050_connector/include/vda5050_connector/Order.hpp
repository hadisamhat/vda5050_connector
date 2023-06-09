#pragma once
#include "vda5050_connector/Edge.hpp"
#include "vda5050_connector/Node.hpp"
#include "vda5050_connector_interface/BaseOrder.hpp"

namespace vda5050_connector {
namespace impl {
class Order : public interface::BaseOrder<Edge, Node> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  std::experimental::optional<Edge> getEdgeWithId(
      const std::string& edgeId, int edgeSequenceId) override;
  std::experimental::optional<Edge> getEdgeWithStartNodeId(
      const std::string& startNodeId, int edgeSequenceId) const override;
  std::experimental::optional<Edge> getEdgeWithEndNodeId(
      const std::string& endNodeId, int edgeSequenceId) const override;
  std::experimental::optional<Node> getNodeWithId(
      const std::string& nodeId, int nodeSequenceId) const override;
  std::experimental::optional<Node> getNodeWithSequenceId(int sequenceId) override;

  std::experimental::optional<Node> getLastReleasedNode() override;

  int getNumberOfReleasedNodes() override;

  int getNumberOfReleasedEdges() override;

  bool hasCorrectNodeCount() override;

  std::experimental::optional<Node> getNextNode(
      const std::string& prevNodeId, int prevNodeSequenceId) const override;
  std::experimental::optional<Node> getPreviousNode(
      const std::string& nodeId, int nodeSequenceId) const override;

  void updateNodeWithId(
      const std::string& nodeId, int nodeSequenceId, const Node& nodeUpdate) override;
  void updateEdgeWithId(
      const std::string& edgeId, int edgeSequenceId, const Edge& edgeUpdate) override;

  // Sorts the order's nodes and edges based on their sequence ID.
  void sort() override;
  // Validates that the nodes and edges follow a correct sequence.
  bool hasValidSequence() override;

  bool isValidOrderUpdate(const Order& orderUpdate);
};
}  // namespace impl
}  // namespace vda5050_connector