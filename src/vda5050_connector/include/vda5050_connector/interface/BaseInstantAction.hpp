#pragma once
#include <experimental/optional>

#include "vda5050_connector/interface/BaseTopicInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class HeaderT, class ActionT>
class BaseInstantAction : public BaseTopicInterface<HeaderT> {
 public:
  std::vector<ActionT> instantActions;
};
}  // namespace interface
}  // namespace vda5050_connector