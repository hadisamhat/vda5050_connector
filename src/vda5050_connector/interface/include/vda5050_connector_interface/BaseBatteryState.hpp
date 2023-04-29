#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseBatteryState : public BaseInterface {
 public:
  float batteryCharge;          // Unit : percentage %, Range : [0, 100]
  float batteryVoltage;         // OPTIONAL, Unit : Voltage V
  float batteryCellMinVoltage;  // Unit : Voltage V
  float batteryCapacityValue;   // Unit : Milliamps x Hours mAH
  int batteryTemperature;       // Unit : Celsius C°
  bool charging;
};

}  // namespace vda5050_connector_interface
}  // namespace iw
