#include "vda5050_connector/EdgeState.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json EdgeState::to_json() {
  return Json{
      {"edgeId", this->edgeId},
      {"sequenceId", this->sequenceId},
      {"edgeDescription", this->edgeDescription},
      {"released", this->released},
      {"trajectory", this->trajectory.to_json()},
  };
}
void EdgeState::from_json(const Json& j) {
  j.at("edgeId").get_to(this->edgeId);
  j.at("sequenceId").get_to(this->sequenceId);
  if (j.find("edgeDescription") != j.end() && !j.at("edgeDescription").is_null()) {
    j.at("edgeDescription").get_to(this->edgeDescription);
  }
  j.at("released").get_to(this->released);
  this->trajectory.from_json(j.at("trajectory"));
}

void EdgeState::fromEdge(const Edge& e) {
  this->edgeId = e.edgeId;
  this->sequenceId = e.sequenceId;
  this->edgeDescription = e.edgeDescription;
  this->trajectory = e.traj;
  this->released = e.released;
}

bool EdgeState::isEqualWithoutTrajectory(const EdgeState& es) const {
  bool res = this->edgeDescription == es.edgeDescription;
  res &= this->edgeId == es.edgeId;
  res &= this->released == es.released;
  res &= this->sequenceId == es.sequenceId;
  return res;
}
}  // namespace impl
}  // namespace vda5050_connector