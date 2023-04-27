#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BasePosition : public BaseInterface {
 public:
  float x;      // OPTIONAL
  float y;      // OPTIONAL
  float theta;  // OPTIONAL
};
}  // namespace interface
}  // namespace vda5050_connector
