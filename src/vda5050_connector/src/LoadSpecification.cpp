#include "vda5050_connector/LoadSpecification.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void LoadSpecification::from_json(const nlohmann::json& j) {
  j.at("loadPositions").get_to(this->loadPositions);

  for (const auto& j : j.at("loadSets")) {
    LoadSet bb;
    bb.from_json(j);
    this->loadSets.push_back(bb);
  }
}
Json LoadSpecification::to_json() {
  Json j = Json{{"loadPositions", this->loadPositions}};
  Json load_sets_j = Json::array();
  for (auto& ls : this->loadSets) {
    load_sets_j.push_back(ls.to_json());
  }
  j["loadSets"] = load_sets_j;
  return j;
}

}  // namespace impl
}  // namespace vda5050_connector