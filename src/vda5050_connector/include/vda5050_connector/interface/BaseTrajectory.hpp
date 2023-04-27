#pragma once
#include <experimental/optional>

#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class ControlPointT>
class BaseTrajectory : public BaseInterface {
 public:
  float degree;                   // Range : [1, INF]
  std::vector<float> knotVector;  // Range : [0.0, 1.0]
  std::vector<ControlPointT> controlPoints;
};

}  // namespace interface
}  // namespace vda5050_connector
