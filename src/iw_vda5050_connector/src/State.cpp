#include "iw_vda5050_connector/State.hpp"

using namespace std;
using Json = nlohmann::json;
using namespace std;
using std::experimental::nullopt;
using std::experimental::optional;

namespace iw {
namespace vda5050 {

Json State::to_json() {
  Json j = Json{{"orderId", this->orderId}, {"orderUpdateId", this->orderUpdateId},
      {"agvVersions", this->agvVersions}, {"lastNodeId", this->lastNodeId},
      {"zoneSetId", this->zoneSetId},
      {"waitingForInteractionZoneRelease", this->waitingForInteractionZoneRelease},
      {"lastNodeSequenceId", this->lastNodeSequenceId},
      {"agvPosition", this->agvPosition.to_json()}, {"velocity", this->velocity.to_json()},
      {"driving", this->driving}, {"paused", this->paused},
      {"newBaseRequest", this->newBaseRequest}, {"batteryState", this->batteryState.to_json()},
      {"operatingMode", this->operatingMode}, {"safetyState", this->safState.to_json()}};
  j.merge_patch(this->header.to_json());
  Json nodeStates_j = Json::array();
  for (auto& n : this->nodeStates) {
    nodeStates_j.push_back(n.to_json());
  }
  j["nodeStates"] = nodeStates_j;
  Json edgeStates_j = Json::array();
  for (auto& e : this->edgeStates) {
    edgeStates_j.push_back(e.to_json());
  }
  j["edgeStates"] = edgeStates_j;
  Json actionStates_j = Json::array();
  for (auto& a : this->actionStates) {
    actionStates_j.push_back(a.to_json());
  }
  j["actionStates"] = actionStates_j;
  Json errors_j = Json::array();
  for (auto& e : this->errors) {
    errors_j.push_back(e.to_json());
  }
  j["errors"] = errors_j;
  Json loads_j = Json::array();
  for (auto& n : this->loads) {
    loads_j.push_back(n.to_json());
  }
  j["loads"] = loads_j;
  Json informations_j = Json::array();
  for (auto& i : this->informations) {
    informations_j.push_back(i.to_json());
  }
  j["informations"] = informations_j;
  Json interactionZones_j = Json::array();
  for (auto& n : this->interactionZones) {
    interactionZones_j.push_back(n.to_json());
  }
  j["interactionZones"] = interactionZones_j;
  return j;
}

void State::from_json(const Json& j) {
  this->header.from_json(j);
  j.at("orderId").get_to(this->orderId);
  j.at("orderUpdateId").get_to(this->orderUpdateId);
  j.at("lastNodeId").get_to(this->lastNodeId);
  j.at("lastNodeSequenceId").get_to(this->lastNodeSequenceId);
  j.at("paused").get_to(this->paused);
  j.at("driving").get_to(this->driving);
  j.at("newBaseRequest").get_to(this->newBaseRequest);
  j.at("operatingMode").get_to(this->operatingMode);
  j.at("waitingForInteractionZoneRelease").get_to(this->waitingForInteractionZoneRelease);
  j.at("zoneSetId").get_to(this->zoneSetId);

  this->agvPosition.from_json(j.at("agvPosition"));
  this->velocity.from_json(j.at("velocity"));
  this->batteryState.from_json(j.at("batteryState"));
  this->safState.from_json(j.at("safetyState"));
  this->agvVersions = j.at("agvVersions");
  for (const auto& j : j.at("nodeStates")) {
    NodeState n;
    n.from_json(j);
    this->nodeStates.push_back(n);
  }
  for (const auto& j : j.at("edgeStates")) {
    EdgeState e;
    e.from_json(j);
    this->edgeStates.push_back(e);
  }
  for (const auto& j : j.at("actionStates")) {
    ActionState as;
    as.from_json(j);
    this->actionStates.push_back(as);
  }
  for (const auto& j : j.at("errors")) {
    Error e;
    e.from_json(j);
    this->errors.push_back(e);
  }
  for (const auto& j : j.at("loads")) {
    Load ls;
    ls.from_json(j);
    this->loads.push_back(ls);
  }
  for (const auto& j : j.at("interactionZones")) {
    InteractionZoneState ls;
    ls.from_json(j);
    this->interactionZones.push_back(ls);
  }
  for (const auto& j : j.at("informations")) {
    Info i;
    i.from_json(j);
    this->informations.push_back(i);
  }
}

/**
 * @brief Get EdgeState from the state with a given edge id and sequence id
 *
 * @param edgeId
 * @param edgeSequenceId
 * @return EdgeState
 */
std::experimental::optional<EdgeState> State::getEdgeStateWithId(
    const std::string& edgeId, int edgeSequenceId) {
  auto esIterator = find_if(this->edgeStates.begin(), this->edgeStates.end(),
      [&](const EdgeState& es) { return es.edgeId == edgeId && es.sequenceId == edgeSequenceId; });
  if (esIterator == this->edgeStates.end()) return std::experimental::nullopt;
  return *esIterator;
}

/**
 * @brief Get NodeState from the state with a given node id and sequence id
 *
 * @param nodeId
 * @param nodeSequenceId
 * @return NodeState
 */
std::experimental::optional<NodeState> State::getNodeStateWithId(
    const std::string& nodeId, int nodeSequenceId) {
  auto nsIterator = find_if(this->nodeStates.begin(), this->nodeStates.end(),
      [&](const NodeState& ns) { return ns.nodeId == nodeId && ns.sequenceId == nodeSequenceId; });
  if (nsIterator == this->nodeStates.end()) return std::experimental::nullopt;
  return *nsIterator;
}

/**
 * @brief Get NodeState from the state with a given sequence id
 *
 * @param sequenceId
 * @return NodeState
 */
std::experimental::optional<NodeState> State::getNodeStateWithSequenceId(int sequenceId) {
  auto it = find_if(this->nodeStates.begin(), this->nodeStates.end(),
      [sequenceId](const NodeState& ns) { return ns.sequenceId == sequenceId; });

  if (it == this->nodeStates.end()) return std::experimental::nullopt;
  return *it;
}

/**
 * @brief Get ActionState from the state with a given action id
 *
 * @param actionId
 * @return ActionState
 */
std::experimental::optional<ActionState> State::getActionStateWithId(const std::string& actionId) {
  auto actionIt = find_if(this->actionStates.begin(), this->actionStates.end(),
      [actionId](const ActionState& as) { return as.actionId == actionId; });
  if (actionIt == this->actionStates.end()) return std::experimental::nullopt;
  return *actionIt;
}

/**
 * @brief Check if the new state contains any significant changes compared to this state
 *
 * @param s new state
 * @return bool, true if: significant attributes like orderId or node States changed
 */
bool State::isSignificantChange(const State& s) {
  bool res = this->orderId != s.orderId;
  res |= this->orderUpdateId != s.orderUpdateId;
  res |= this->nodeStates != s.nodeStates;
  if (this->edgeStates.size() == s.edgeStates.size()) {
    res |= !std::equal(this->edgeStates.begin(), this->edgeStates.end(), s.edgeStates.begin(),
        [](const EdgeState& es1, const EdgeState& es2) {
          return es1.isEqualWithoutTrajectory(es2);
        });
  } else {
    res |= this->edgeStates.size() != s.edgeStates.size();
  }
  res |= this->actionStates != s.actionStates;
  res |= this->driving != s.driving;
  res |= this->errors != s.errors;
  res |= this->operatingMode != s.operatingMode;

  return res;
}

/**
 * @brief Check if node and sequence id continue on the ones of the previous order
 *
 * @param orderUpdate
 * @return bool
 */
bool State::updateContinuesOnPreviousOrder(const Order& orderUpdate) {
  if (this->nodeStates.empty()) {
    if ((orderUpdate.nodes.front().nodeId == this->lastNodeId) &&
        (orderUpdate.nodes.front().sequenceId == this->lastNodeSequenceId)) {
      return true;
    }
  }

  return false;
}

/**
 * @brief Check if the state has a horizon (unreleased node states)
 *
 * @return bool, true if: state has a horizon
 */
bool State::hasHorizon() {
  bool hasHorizon = false;
  for (const auto& ns : this->nodeStates) {
    if (!ns.released) {
      hasHorizon = true;
      break;
    }
  }
  return hasHorizon;
}

/**
 * Count the number of released states
 *
 * @return int: Returns number of released nodes
 */
int State::getNumberOfReleasedNodes() {
  int baseNodes = count_if(this->nodeStates.begin(), this->nodeStates.end(),
      [](const NodeState& ns) { return ns.released; });
  return baseNodes;
}

/**
 * @brief Find action state with the given action id and remove it
 *
 * @param actionId
 * @return bool: true if removing action state was successful
 */
bool State::removeActionStateWithId(const std::string& actionId) {
  auto actionIt = find_if(this->actionStates.begin(), this->actionStates.end(),
      [actionId](const ActionState& as) { return as.actionId == actionId; });
  // The action id for the action was found, remove it
  if (actionIt != this->actionStates.end()) {
    this->actionStates.erase(actionIt);
    return true;
  }
  return false;
}

/**
 * @brief Find node state with the given node and node-sequence id and remove it
 *
 * @param nodeId
 * @param nodeSequenceId
 * @return bool: true if removing node was successful
 */
bool State::removeNodeStateWithId(const std::string& nodeId, int nodeSequenceId) {
  auto nsIterator = find_if(this->nodeStates.begin(), this->nodeStates.end(),
      [&](const NodeState& ns) { return ns.nodeId == nodeId && ns.sequenceId == nodeSequenceId; });
  // The action id for the action was found, remove it
  if (nsIterator != this->nodeStates.end()) {
    this->nodeStates.erase(nsIterator);
    return true;
  }
  return false;
}

/**
 * @brief Find edge state with the given edge and edge-sequence id and remove it
 *
 * @param edgeId
 * @param edgeSequenceId
 * @return bool: true if removing edge was was successful
 */
bool State::removeEdgeStateWithId(const std::string& edgeId, int edgeSequenceId) {
  auto esIterator = find_if(this->edgeStates.begin(), this->edgeStates.end(),
      [&](const EdgeState& es) { return es.edgeId == edgeId && es.sequenceId == edgeSequenceId; });
  // The action id for the action was found, remove it
  if (esIterator != this->edgeStates.end()) {
    this->edgeStates.erase(esIterator);
    return true;
  }
  return false;
}

/**
 * @brief Update trajectory of the edge state: Find edge by edge- and edge-sequence id and replace
 * the trajectory
 *
 * @param edgeId
 * @param edgeSequenceId
 * @param traj
 */
void State::updateEdgeStateTrajectory(
    const std::string& edgeId, int edgeSequenceId, Trajectory traj) {
  auto esIterator = find_if(this->edgeStates.begin(), this->edgeStates.end(),
      [&](const EdgeState& es) { return es.edgeId == edgeId && es.sequenceId == edgeSequenceId; });
  if (esIterator == this->edgeStates.end()) return;

  esIterator->trajectory = traj;
}

/**
 * @brief Update node state of a given id: Find node by node- and node-sequence id and update it
 *
 * @param nodeId
 * @param nodeSequenceId
 * @param nodeUpdate
 */
void State::updateNodeStateWithId(
    const std::string& nodeId, int nodeSequenceId, const Node& nodeUpdate) {
  auto it = find_if(this->nodeStates.begin(), this->nodeStates.end(),
      [&](const NodeState& n) { return n.nodeId == nodeId && n.sequenceId == nodeSequenceId; });
  if (it != this->nodeStates.end()) it->fromNode(nodeUpdate);
}

/**
 * @brief Update edge state of a given id: Find edge by edge- and edge-sequence id and update it
 *
 * @param nodeId
 * @param nodeSequenceId
 * @param nodeUpdate
 */
void State::updateEdgeStateWithId(
    const std::string& edgeId, int edgeSequenceId, const Edge& edgeUpdate) {
  auto it = find_if(this->edgeStates.begin(), this->edgeStates.end(),
      [&](const EdgeState& e) { return e.edgeId == edgeId && e.sequenceId == edgeSequenceId; });
  if (it != this->edgeStates.end()) it->fromEdge(edgeUpdate);
}

/**
 * @brief Update action state of a given id: Find action by action id and replace the status
 *
 * @param actionId
 * @param newStatus
 */
void State::updateActionStateWithId(const std::string& actionId, const std::string& newStatus) {
  auto it = find_if(this->actionStates.begin(), this->actionStates.end(),
      [actionId](const ActionState& a) { return a.actionId == actionId; });
  if (it != this->actionStates.end()) it->actionStatus = newStatus;
}

/**
 * Update action result of a given action id: Find action by action id and set new result
 * description
 *
 * @param actionId
 * @param result
 */
void State::updateActionResultWithId(const std::string& actionId, const std::string& result) {
  auto it = find_if(this->actionStates.begin(), this->actionStates.end(),
      [actionId](const ActionState& a) { return a.actionId == actionId; });
  if (it != this->actionStates.end()) it->resultDescription = result;
}

/**
 * @brief Check remaining actions in the order and if there are any remaining nodes to traverse by
 * checking the actionStatus.
 *
 * @return bool: true if there are are remaining orders and nodes
 */
bool State::hasActiveOrder() const {
  // Check remaining actions in the order and if there are any remaining nodes to traverse.
  int remaining_actions =
      count_if(this->actionStates.begin(), this->actionStates.end(), [](const ActionState& as) {
        return as.actionStatus == vda5050_connector::interface::ActionStates::INITIALIZING ||
               as.actionStatus == vda5050_connector::interface::ActionStates::WAITING ||
               as.actionStatus == vda5050_connector::interface::ActionStates::RUNNING ||
               as.actionStatus == vda5050_connector::interface::ActionStates::PAUSED;
      });
  return !(this->nodeStates.empty() && remaining_actions == 0);
}

}  // namespace vda5050
}  // namespace iw