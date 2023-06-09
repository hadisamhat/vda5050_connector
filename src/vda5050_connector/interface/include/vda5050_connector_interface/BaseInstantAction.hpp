#pragma once
#include <experimental/optional>

#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class ActionT>
class BaseInstantAction : public BaseInterface {
 public:
  std::vector<ActionT> instantActions;
};
}  // namespace interface
}  // namespace vda5050_connector