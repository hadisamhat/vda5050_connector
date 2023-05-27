#pragma once
#if __has_include(<optional>)
#include <optional>
#elif __has_include(<experimental/optional>)
#include <experimental/optional>
namespace std {
using namespace experimental;
}
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
