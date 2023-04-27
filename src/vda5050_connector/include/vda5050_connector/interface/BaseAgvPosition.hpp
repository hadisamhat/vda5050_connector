#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseAgvPosition : public BaseInterface {
 public:
  bool positionInitialized;
  float localizationScore;  // OPTIONAL
  float deviationRange;     // OPTIONAL, Unit : meters
  float x;                  // Unit : meters
  float y;                  // Unit : meters
  float theta;              // Unit : radians , Range : [-Pi, Pi]
  std::string mapId;
  std::string mapDescription;  // OPTIONAL
};
}  // namespace interface
}  // namespace vda5050_connector
