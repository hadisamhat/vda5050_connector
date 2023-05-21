#include "vda5050_connector_interface/BaseInterface.hpp"

namespace iw {
namespace vda5050 {

class InteractionZoneState : public vda5050_connector::interface::BaseInterface {
 public:
  std::string zoneId;
  int status;
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  InteractionZoneState();
};
}  // namespace vda5050_connector
}  // namespace iw