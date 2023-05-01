#include "vda5050_connector/Envelope2d.hpp"

using namespace std;
using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {

Json Envelope2d::to_json() {
  Json j = Json{{"set", this->set}, {"description", this->description}};
  Json polygon_j = Json::array();
  for (auto& pt : this->polygonPoints) {
    polygon_j.push_back(pt.to_json());
  }
  j["polygonPoints"] = polygon_j;
  return j;
}

void Envelope2d::from_json(const Json& j) {
  j.at("set").get_to(this->set);
  j.at("description").get_to(this->description);
  for (const auto& j : j.at("polygonPoints")) {
    PolygonPoint p;
    p.from_json(j);
    this->polygonPoints.push_back(p);
  }
}
}  // namespace impl
}  // namespace vda5050_connector