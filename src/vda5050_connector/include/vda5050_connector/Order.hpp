#pragma once
#include "vda5050_connector/Edge.hpp"
#include "vda5050_connector/Node.hpp"
#include "vda5050_connector/ProtocolHeader.hpp"
#include "vda5050_connector_interface/BaseOrder.hpp"

namespace vda5050_connector {
namespace impl {
class Order : public interface::BaseOrder<ProtocolHeader, Edge, Node> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  std::optional<Edge> getEdgeWithId(
      const std::string& edgeId, const int& edgeSequenceId) override;
  std::optional<Edge> getEdgeWithStartNodeId(
      const std::string& startNodeId, const int& edgeSequenceId) const override;
  std::optional<Edge> getEdgeWithEndNodeId(
      const std::string& endNodeId, const int& edgeSequenceId) const override;
  std::optional<Node> getNodeWithId(
      const std::string& nodeId, const int& nodeSequenceId) const override;
  std::optional<Node> getNodeWithSequenceId(int sequenceId) override;

  std::optional<Node> getLastReleasedNode() override;

  int getNumberOfReleasedNodes() override;

  int getNumberOfReleasedEdges() override;

  bool hasCorrectNodeCount() override;

  std::optional<Node> getNextNode(
      const std::string& prevNodeId, const int& prevNodeSequenceId) const override;
  std::optional<Node> getPreviousNode(
      const std::string& nodeId, const int& nodeSequenceId) const override;

  void updateNodeWithId(
      const std::string& nodeId, const int& nodeSequenceId, const Node& nodeUpdate) override;
  void updateEdgeWithId(
      const std::string& edgeId, const int& edgeSequenceId, const Edge& edgeUpdate) override;

  // Sorts the order's nodes and edges based on their sequence ID.
  void sort() override;
  // Validates that the nodes and edges follow a correct sequence.
  bool hasValidSequence() override;

  bool isValidOrderUpdate(const Order& orderUpdate);
};
}  // namespace impl
}  // namespace vda5050_connector