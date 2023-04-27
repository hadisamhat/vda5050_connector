#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseNodePostion : public BaseInterface {
 public:
  float x;                      // Unit : meters
  float y;                      // Unit : meters
  float theta;                  // OPTIONAL, Unit : radians, Range : [-Pi, Pi]
  float allowedDeviationXY;     // OPTIONAL
  float allowedDeviationTheta;  // OPTIONAL, Range : [0, Pi]
  std::string mapId;
  std::string mapDescription;  // OPTIONAL
};

}  // namespace interface
}  // namespace vda5050_connector
