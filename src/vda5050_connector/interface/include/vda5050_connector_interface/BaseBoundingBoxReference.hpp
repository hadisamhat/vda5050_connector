#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseBoundingBoxReference : public BaseInterface {
 public:
  float x;
  float y;
  float z;
  float theta;
};
}  // namespace interface
}  // namespace vda5050_connector
