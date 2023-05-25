#pragma once
#if __cplusplus < 201402L
#include <experimental/optional>
namespace std {
using optional = experimental::optional
}
#else
#include <optional>
#endif
#include "vda5050_connector_interface/BaseInterface.hpp"

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
