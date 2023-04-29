#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BasePolygonPoint : public BaseInterface {
 public:
  double x;  // OPTIONAL
  double y;  // OPTIONAL
};
}  // namespace interface
}  // namespace vda5050_connector
