#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class NodeT, class NodePositionT>
class BaseNodeState : public BaseInterface {
 public:
  std::string nodeId;
  int sequenceId;
  std::string nodeDescription;  // OPTIONAL
  NodePositionT nodePosition;
  bool released;

  virtual void fromNode(const NodeT& n) = 0;
};

}  // namespace interface
}  // namespace vda5050_connector
