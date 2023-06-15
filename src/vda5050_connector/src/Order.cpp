#include "vda5050_connector/Order.hpp"

using Json = nlohmann::json;
using namespace std;
using std::nullopt;
using std::optional;

namespace vda5050_connector {
namespace impl {
Json Order::to_json() {
  Json j = Json{{"orderId", this->orderId}, {"orderUpdateId", this->orderUpdateId},
      {"zoneSetId", this->zoneSetId}};
  Json nodes_j = Json::array();
  for (auto& n : this->nodes) {
    nodes_j.push_back(n.to_json());
  }
  j["nodes"] = nodes_j;
  Json edge_j = Json::array();
  for (auto& e : this->edges) {
    edge_j.push_back(e.to_json());
  }
  j["edges"] = edge_j;
  return j;
}

void Order::from_json(const Json& j) {
  j.at("orderId").get_to(this->orderId);
  j.at("orderUpdateId").get_to(this->orderUpdateId);
  if (j.find("zoneSetId") != j.end() && !j.at("zoneSetId").is_null()) {
    j.at("zoneSetId").get_to(this->zoneSetId);
  }
  for (const auto& j : j.at("nodes")) {
    Node n;
    n.from_json(j);
    this->nodes.push_back(n);
  }
  for (const auto& j : j.at("edges")) {
    Edge e;
    e.from_json(j);
    this->edges.push_back(e);
  }
}

/**
 * @brief Get edge with the given edge and edge-sequence id
 *
 * @param edgeId
 * @param edgeSequenceId
 * @return Edge
 */
std::optional<Edge> Order::getEdgeWithId(
    const std::string& edgeId, int edgeSequenceId) {
  auto it = find_if(this->edges.begin(), this->edges.end(),
      [&](const Edge& e) { return e.edgeId == edgeId && e.sequenceId == edgeSequenceId; });

  if (it == this->edges.end()) return std::nullopt;
  return *it;
}

/**
 * @brief Get edge with the given start-node and edge-sequence id
 *
 * @param startNodeId
 * @param edgeSequenceId
 * @return Edge
 */
std::optional<Edge> Order::getEdgeWithStartNodeId(
    const std::string& startNodeId, int edgeSequenceId) const {
  auto orderIt = find_if(this->edges.begin(), this->edges.end(), [&](const Edge& e) {
    return e.startNodeId == startNodeId && e.sequenceId == edgeSequenceId;
  });
  if (orderIt == this->edges.end()) return std::nullopt;
  return *orderIt;
}

/**
 * @brief Get edge with the given end-node and edge-sequence id
 *
 * @param endNodeId
 * @param edgeSequenceId
 * @return Edge
 */
std::optional<Edge> Order::getEdgeWithEndNodeId(
    const std::string& endNodeId, int edgeSequenceId) const {
  auto orderIt = find_if(this->edges.begin(), this->edges.end(),
      [&](const Edge& e) { return e.endNodeId == endNodeId && e.sequenceId == edgeSequenceId; });
  if (orderIt == this->edges.end()) return std::nullopt;
  return *orderIt;
}

/**
 * @brief Get node with the given node and node-sequence id
 *
 * @param nodeId
 * @param nodeSequenceId
 * @return Node
 */
std::optional<Node> Order::getNodeWithId(
    const std::string& nodeId, int nodeSequenceId) const {
  auto it = find_if(this->nodes.begin(), this->nodes.end(),
      [&](const Node& n) { return n.nodeId == nodeId && n.sequenceId == nodeSequenceId; });

  if (it == this->nodes.end()) return std::nullopt;
  return *it;
}

/**
 * @brief Get node with the given sequence id
 *
 * @param sequenceId
 * @return Node
 */
std::optional<Node> Order::getNodeWithSequenceId(int sequenceId) {
  auto it = find_if(this->nodes.begin(), this->nodes.end(),
      [sequenceId](const Node& n) { return n.sequenceId == sequenceId; });

  if (it == this->nodes.end()) return std::nullopt;
  return *it;
}

/**
 * @brief Get the last released node of this order
 *
 * @return Node
 */
std::optional<Node> Order::getLastReleasedNode() {
  auto it = find_if(reverse_iterator<vector<Node>::iterator>(nodes.end()),
      reverse_iterator<vector<Node>::iterator>(nodes.begin()),
      [](const Node& n) { return n.released; });

  if (it == reverse_iterator<vector<Node>::iterator>(nodes.begin()))
    return std::nullopt;
  return *it;
}

/**
 * @brief Check if first node of the order update is the last released node in the current order.
 * If true, the update is valid
 *
 * @param orderUpdate
 * @return bool
 */
bool Order::isValidOrderUpdate(const Order& orderUpdate) {
  // Get last released node
  auto lastReleasedNode = this->getLastReleasedNode();
  if (!lastReleasedNode) return false;
  // Check if the first node in the update is the last released node in the current running order.
  return ((orderUpdate.nodes.front().nodeId == lastReleasedNode.value().nodeId) &&
          (orderUpdate.nodes.front().sequenceId == lastReleasedNode.value().sequenceId));
}

/**
 * @brief Get number or released nodes in this order
 *
 * @return int
 */
int Order::getNumberOfReleasedNodes() {
  int releasedNodes =
      count_if(this->nodes.begin(), this->nodes.end(), [](Node& n) { return n.released; });
  return releasedNodes;
}

/**
 * @brief Get number or released edges in this order
 *
 * @return int
 */
int Order::getNumberOfReleasedEdges() {
  int releasedEdges =
      count_if(this->edges.begin(), this->edges.end(), [](Edge& e) { return e.released; });
  return releasedEdges;
}

/**
 * @brief Verify that Order has correct number of nodes
 * Number of edges = number of nodes -1 for both only released and all nodes
 *
 * @return bool
 */
bool Order::hasCorrectNodeCount() {
  // Check if the number of nodes and edges is correct in the
  // order.
  int releasedNodes = getNumberOfReleasedNodes();
  int releasedEdges = getNumberOfReleasedEdges();

  return ((releasedEdges == releasedNodes - 1) && (this->edges.size() == this->nodes.size() - 1));
}

/**
 * @brief Get next/ following node of the given previous node by finding the the connecting edge
 *
 * @param prevNodeId
 * @param prevNodeSequenceId
 * @return nullopt if not next node exists
 * @return Node
 */
std::optional<Node> Order::getNextNode(
    const std::string& prevNodeId, int prevNodeSequenceId) const {
  auto edgeIt = this->getEdgeWithStartNodeId(prevNodeId, prevNodeSequenceId + 1);
  if (!edgeIt) return std::nullopt;

  // Find the node that is connected to the edge found above.
  auto nodeIt = this->getNodeWithId(edgeIt.value().endNodeId, edgeIt.value().sequenceId + 1);
  if (!nodeIt) return std::nullopt;

  return *nodeIt;
}

/**
 * @brief Update node of a given node and node-sequence id
 *
 * @param nodeId
 * @param nodeSequenceId
 * @param nodeUpdate
 */
void Order::updateNodeWithId(
    const std::string& nodeId, int nodeSequenceId, const Node& nodeUpdate) {
  auto it = find_if(this->nodes.begin(), this->nodes.end(),
      [&](const Node& n) { return n.nodeId == nodeId && n.sequenceId == nodeSequenceId; });
  if (it != this->nodes.end()) it->fromNode(nodeUpdate);
}

/**
 * @brief Update Edge of a given edge and edge-sequence id
 *
 * @param edgeId
 * @param edgeSequenceId
 * @param edgeUpdate
 */
void Order::updateEdgeWithId(
    const std::string& edgeId, int edgeSequenceId, const Edge& edgeUpdate) {
  auto it = find_if(this->edges.begin(), this->edges.end(),
      [&](const Edge& n) { return n.edgeId == edgeId && n.sequenceId == edgeSequenceId; });
  if (it != this->edges.end()) it->fromEdge(edgeUpdate);
}

/**
 * @brief Return the previous node of this order, by finding the connecting edge
 *
 * @param nodeId
 * @param nodeSequenceId
 * @return nullopt if not previous node exists
 * @return Node
 */
std::optional<Node> Order::getPreviousNode(
    const string& nodeId, int nodeSequenceId) const {
  auto edgeIt = this->getEdgeWithEndNodeId(nodeId, nodeSequenceId - 1);
  if (!edgeIt) return std::nullopt;

  // Find the node that is connected to the edge found above.
  auto nodeIt = this->getNodeWithId(edgeIt.value().startNodeId, edgeIt.value().sequenceId - 1);
  if (!nodeIt) return std::nullopt;

  return *nodeIt;
}

/**
 * @brief Sorts the nodes and edges in the order based on the sequence IDs.
 */
void Order::sort() {
  std::sort(this->nodes.begin(), this->nodes.end(),
      [](const Node& n1, const Node& n2) { return n1.sequenceId < n2.sequenceId; });
  std::sort(this->edges.begin(), this->edges.end(),
      [](const Edge& e1, const Edge& e2) { return e1.sequenceId < e2.sequenceId; });
}

/**
 * @brief Validates that the nodes and edges in the order are in correct sequence, well connected
 * and have correct base/horizon logic. This method assumes that the nodes and edges are sorted.
 *
 * @return True if the order follows a correct sequence
 */
bool Order::hasValidSequence() {
  // Order can't be without any nodes.
  if (this->nodes.empty()) return false;

  // If there's only one node, the node has to be released.
  if (this->nodes.size() == 1) return this->nodes[0].released;

  Node last_node;
  bool horizon = !this->edges[0].released;
  for (size_t i = 0; i < this->nodes.size() - 1; ++i) {
    // If the edge number to be checked is outside of the range, then it's a malformed order.
    if (i == this->edges.size()) return false;
    auto current_node = this->nodes[i];
    auto next_node = this->nodes[i + 1];
    auto next_edge = this->edges[i];

    // While in horizon, edges and nodes cannot be released.
    if (horizon && (next_edge.released || next_node.released)) return false;

    // Check if the next edge and node belong to the same base/horizon.
    if (next_edge.released != next_node.released) return false;

    // Validate that the current node is connected to the next node via the edge.
    if ((current_node.nodeId != next_edge.startNodeId) ||
        (next_node.nodeId != next_edge.endNodeId) ||
        (next_edge.sequenceId != current_node.sequenceId + 1) ||
        (next_node.sequenceId != next_edge.sequenceId + 1))
      return false;
    // Update the horizon value if the iteration enters the horizon of the order.
    if (!horizon && !next_edge.released) horizon = true;
  }
  return true;
}

}  // namespace impl
}  // namespace vda5050_connector