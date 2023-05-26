#pragma once
#if __cplusplus < 201703L
#include <experimental/optional>
namespace std {
using optional = experimental::optional;
}
#else
#include <optional>
#endif
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class NodePositionT, class ActionT>
class BaseNode : public BaseInterface {
 public:
  std::string nodeId;
  int sequenceId;
  std::string nodeDescription;  // OPTIONAL
  bool released;
  NodePositionT nodePosition;
  std::vector<ActionT> actions;

  virtual std::optional<ActionT> tryGetActionOfType(const std::string& action_type) = 0;
  virtual bool hasActionOfType(const std::string& action_type) const = 0;
  virtual std::optional<std::string> getTypeFromDescription() const = 0;
};

}  // namespace interface
}  // namespace vda5050_connector
