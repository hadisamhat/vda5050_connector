#pragma once
#include <boost/algorithm/string.hpp>
#if __cplusplus > 201402L
#include <optional>
#else
#include <experimental/optional>
namespace std {
using namespace experimental;
}
#endif
#include "vda5050_connector_interface/BaseActionParameter.hpp"

namespace vda5050_connector {
namespace impl {
class ActionParameter : public interface::BaseActionParameter {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  bool operator==(const ActionParameter& ap) const;

 private:
  void UpperFirst(std::string& s);
};
}  // namespace impl
}  // namespace vda5050_connector