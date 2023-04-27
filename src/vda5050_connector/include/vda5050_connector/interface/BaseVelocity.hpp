#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseVelocity : public BaseInterface {
 public:
  float vx;     // OPTIONAL
  float vy;     // OPTIONAL
  float omega;  // OPTIONAL
};
}  // namespace interface
}  // namespace vda5050_connector
