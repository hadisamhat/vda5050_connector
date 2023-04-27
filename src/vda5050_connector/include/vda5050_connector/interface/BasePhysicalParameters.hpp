#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BasePhysicalParameters : public BaseInterface {
 public:
  float speedMin;
  float speedMax;
  float accelerationMax;
  float decelerationMax;
  float heightMin;
  float heightMax;
  float width;
  float length;
};

}  // namespace interface
}  // namespace vda5050_connector
