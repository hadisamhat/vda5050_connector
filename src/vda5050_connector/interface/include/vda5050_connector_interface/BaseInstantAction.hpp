#pragma once
#if __cplusplus < 201703L
#include <experimental/optional>
namespace std {
namespace experimental {}
using namespace experimental;
}  // namespace std
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