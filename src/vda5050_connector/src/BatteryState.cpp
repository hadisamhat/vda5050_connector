#include "vda5050_connector/BatteryState.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace impl {
void BatteryState::from_json(const nlohmann::json& j) {
  j.at("batteryCharge").get_to(this->batteryCharge);
  j.at("batteryVoltage").get_to(this->batteryVoltage);
  j.at("charging").get_to(this->charging);
  j.at("batteryCellMinVoltage").get_to(this->batteryCellMinVoltage);
  j.at("batteryCapacityValue").get_to(this->batteryCapacityValue);
  j.at("batteryTemperature").get_to(this->batteryTemperature);
}
Json BatteryState::to_json() {
  return Json{{"batteryCharge", this->batteryCharge}, {"batteryVoltage", this->batteryVoltage},
      {"charging", this->charging}, {"batteryCellMinVoltage", this->batteryCellMinVoltage},
      {"batteryCapacityValue", this->batteryCapacityValue},
      {"batteryTemperature", this->batteryTemperature}};
}
}  // namespace impl
}  // namespace vda5050_connector