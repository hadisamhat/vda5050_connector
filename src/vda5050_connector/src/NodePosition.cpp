#include "vda5050_connector/NodePosition.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json NodePosition::to_json() {
  return Json{{"x", this->x}, {"y", this->y}, {"theta", this->theta},
      {"allowedDeviationXY", this->allowedDeviationXY},
      {"allowedDeviationTheta", this->allowedDeviationTheta}, {"mapId", this->mapId},
      {"mapDescription", this->mapDescription}};
}

void NodePosition::from_json(const Json& j) {
  j.at("x").get_to(this->x);
  j.at("y").get_to(this->y);
  if (j.find("theta") != j.end() && !j.at("theta").is_null()) {
    j.at("theta").get_to(this->theta);
  }
  // If allowedDeviation does not exist in the order, set it to -1 in order to compare when handling
  // the order
  if (j.find("allowedDeviationXY") != j.end() && !j.at("allowedDeviationXY").is_null()) {
    j.at("allowedDeviationXY").get_to(this->allowedDeviationXY);
  } else {
    this->allowedDeviationXY = -1.0;
  }
  if (j.find("allowedDeviationTheta") != j.end() && !j.at("allowedDeviationTheta").is_null()) {
    j.at("allowedDeviationTheta").get_to(this->allowedDeviationTheta);
  } else {
    this->allowedDeviationXY = -1.0;
  }
  j.at("mapId").get_to(this->mapId);
  if (j.find("mapDescription") != j.end() && !j.at("mapDescription").is_null()) {
    j.at("mapDescription").get_to(this->mapDescription);
  }
}

bool NodePosition::operator==(const NodePosition& np) const {
  bool res = this->x == np.x && this->y == np.y;
  res &= this->theta == np.theta;
  res &= this->allowedDeviationXY == np.allowedDeviationXY;
  res &= this->allowedDeviationTheta == np.allowedDeviationTheta;
  res &= this->mapId == np.mapId;
  res &= this->mapDescription == np.mapDescription;
  return res;
}
}  // namespace impl
}  // namespace vda5050_connector