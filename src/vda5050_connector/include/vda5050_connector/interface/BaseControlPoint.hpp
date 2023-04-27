#pragma once
#include <experimental/optional>

#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseControlPoint : public BaseInterface {
 public:
  float x;            // Unit : meters
  float y;            // unit : meters
  float orientation;  // OPTIONAL, Unit : radians, Range : [-Pi, Pi]
  float weight;       // Range : [0, INF]
};

}  // namespace interface
}  // namespace vda5050_connector
