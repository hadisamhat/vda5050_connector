#pragma once
#if __cplusplus < 201402L
#include <experimental/optional>
namespace std {
using optional = experimental::optional
}
#else
#include <optional>
#endif
#include "vda5050_connector_interface/BaseTopicInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class HeaderT, class ActionT>
class BaseInstantAction : public BaseTopicInterface<HeaderT> {
 public:
  std::vector<ActionT> instantActions;
};
}  // namespace interface
}  // namespace vda5050_connector