#include <experimental/optional>

#include "iw_vda5050_connector/ZoneParameter.hpp"
#include "vda5050_connector/PolygonPoint.hpp"
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace iw {
namespace vda5050 {

class Zone : public vda5050_connector::interface::BaseInterface {
 public:
  std::string zoneId;
  std::string zoneType;
  std::string mapId;
  std::string zoneName;         // OPTIONAL
  std::string zoneDescription;  // OPTIONAL
  std::vector<ZoneParameter> zoneParameters;
  std::vector<vda5050_connector::impl::PolygonPoint> polygon;

  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;

  template <typename T>
  std::experimental::optional<T> getZoneParameterByKey(const char* key) const {
    auto it = find_if(this->zoneParameters.begin(), this->zoneParameters.end(),
        [key](const ZoneParameter z1) { return z1.key == key; });
    if (it != this->zoneParameters.end()) {
      std::istringstream ss(it->value);
      T value;
      ss >> value;
      return value;
    }
    return std::experimental::nullopt;
    ;
  }
};
}  // namespace vda5050
}  // namespace iw