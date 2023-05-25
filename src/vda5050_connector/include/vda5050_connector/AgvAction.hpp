#pragma once
#include "vda5050_connector/ActionParameter.hpp"
#include "vda5050_connector_interface/BaseAgvAction.hpp"

namespace vda5050_connector {
namespace impl {

class AgvAction : public interface::BaseAgvAction<ActionParameter> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;

 private:
  void UpperFirst(std::string& s);
};
}  // namespace impl
}  // namespace vda5050_connector