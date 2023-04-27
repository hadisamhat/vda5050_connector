#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class EdgeT, class TrajectoryT>
class BaseEdgeState : public BaseInterface {
 public:
  std::string edgeId;
  int sequenceId;
  std::string edgeDescription;  // OPTIONAL
  bool released;
  TrajectoryT trajectory;

  bool isEqualWithoutTrajectory(const EdgeStateT& es1) const;
  void fromEdge(const EdgeT& e);
};

}  // namespace interface
}  // namespace vda5050_connector
