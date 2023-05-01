#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class TrajectoryT>
class BaseEdgeState : public BaseInterface {
 public:
  std::string edgeId;
  int sequenceId;
  std::string edgeDescription;  // OPTIONAL
  bool released;
  TrajectoryT trajectory;
};

}  // namespace interface
}  // namespace vda5050_connector
