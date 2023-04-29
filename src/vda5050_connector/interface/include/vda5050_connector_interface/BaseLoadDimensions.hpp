#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseLoadDimensions : public BaseInterface {
 public:
  float length;  // Unit : meters
  float width;   // Unit : meters
  float height;  // OPTIONAL, Unit : meters
};
}  // namespace interface
}  // namespace vda5050_connector