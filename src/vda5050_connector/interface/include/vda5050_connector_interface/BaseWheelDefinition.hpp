#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

namespace WheelType {
constexpr char DRIVE[] = "DRIVE";
constexpr char CASTER[] = "CASTER";
constexpr char FIXED[] = "FIXED";
constexpr char MECANUM[] = "MECANUM";
};  // namespace WheelType

template <class PositionT>
class BaseWheelDefinition : public BaseInterface {
 public:
  std::string type;
  bool isActiveDriven;
  bool isActiveSteered;
  PositionT position;
  float diameter;
  float width;
  float centerDisplacement;
  std::string constraints;
};

}  // namespace interface
}  // namespace vda5050_connector
