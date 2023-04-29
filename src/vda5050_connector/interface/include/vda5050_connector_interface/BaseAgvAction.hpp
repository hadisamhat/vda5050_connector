#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

namespace ActionScopes {
constexpr char INSTANT[] = "INSTANT";
constexpr char NODE[] = "NODE";
constexpr char EDGE[] = "EDGE";
};  // namespace ActionScopes

template <class ActionParameterT>
class BaseAgvAction : public BaseInterface {
 public:
  std::string actionType;
  std::string actionScopes;
  std::string actionDescription;
  std::vector<ActionParameterT> actionParameters;
};

}  // namespace interface
}  // namespace vda5050_connector
