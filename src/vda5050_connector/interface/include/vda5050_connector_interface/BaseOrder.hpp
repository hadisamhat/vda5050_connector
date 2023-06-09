#pragma once
#include <experimental/optional>

#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class EdgeT, class NodeT>
class BaseOrder : public BaseInterface {
 public:
  std::string orderId;
  int orderUpdateId;
  std::string zoneSetId;  // OPTIONAL
  std::vector<NodeT> nodes;
  std::vector<EdgeT> edges;

  virtual std::experimental::optional<EdgeT> getEdgeWithId(
      const std::string& edgeId, int edgeSequenceId) = 0;
  virtual std::experimental::optional<EdgeT> getEdgeWithStartNodeId(
      const std::string& startNodeId, int edgeSequenceId) const = 0;
  virtual std::experimental::optional<EdgeT> getEdgeWithEndNodeId(
      const std::string& endNodeId, int edgeSequenceId) const = 0;

  virtual std::experimental::optional<NodeT> getNodeWithId(
      const std::string& nodeId, int nodeSequenceId) const = 0;
  virtual std::experimental::optional<NodeT> getNodeWithSequenceId(int sequenceId) = 0;

  virtual std::experimental::optional<NodeT> getLastReleasedNode() = 0;

  virtual int getNumberOfReleasedNodes() = 0;

  virtual int getNumberOfReleasedEdges() = 0;

  virtual bool hasCorrectNodeCount() = 0;

  virtual std::experimental::optional<NodeT> getNextNode(
      const std::string& prevNodeId, int prevNodeSequenceId) const = 0;
  virtual std::experimental::optional<NodeT> getPreviousNode(
      const std::string& nodeId, int nodeSequenceId) const = 0;

  virtual void updateNodeWithId(
      const std::string& nodeId, int nodeSequenceId, const NodeT& nodeUpdate) = 0;
  virtual void updateEdgeWithId(
      const std::string& edgeId, int edgeSequenceId, const EdgeT& edgeUpdate) = 0;

  // Sorts the order's nodes and edges based on their sequence ID.
  virtual void sort() = 0;
  // Validates that the nodes and edges follow a correct sequence.
  virtual bool hasValidSequence() = 0;
};
}  // namespace interface
}  // namespace vda5050_connector