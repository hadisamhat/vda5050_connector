#pragma once
#if __has_include(<optional>)
#include <optional>
#elif __has_include(<experimental/optional>)
#include <experimental/optional>
namespace std {
using namespace experimental;
}
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