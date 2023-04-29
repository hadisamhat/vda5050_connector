#pragma once
#include <boost/algorithm/string.hpp>
#include <experimental/optional>

#include "vda5050_connector_interface/BaseActionParameter.hpp"

namespace vda5050_connector {
namespace impl {
class ActionParameter : public interface::BaseActionParameter {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  bool operator==(const ActionParameter& ap) const;

 private:
  std::string UpperFirst(std::string s);
};
}  // namespace impl
}  // namespace vda5050_connector