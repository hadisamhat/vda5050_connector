#include "iw_vda5050_connector/Action.hpp"
#include "iw_vda5050_connector/InstantAction.hpp"
#include "iw_vda5050_connector/MapUpdate.hpp"
#include "iw_vda5050_connector/NetworkConfiguration.hpp"
#include "iw_vda5050_connector/Visualization.hpp"
#include "iw_vda5050_connector/ZoneUpdate.hpp"
#include "vda5050_connector/Connection.hpp"
#include "vda5050_connector/FactSheet.hpp"
#include "vda5050_connector/ManagerFSM.hpp"
#include "vda5050_connector/Order.hpp"
#include "vda5050_connector/State.hpp"

using namespace vda5050_connector::impl;
using namespace vda5050_connector::interface;

namespace iw {
namespace vda5050 {
class Manager
    : public ManagerFSM<Order, InstantAction, State, Visualization, Connection, FactSheet> {
 public:
  Manager(NetworkConfiguration config);

  void start() override;
  MapUpdate getMapUpdateMsg();
  ZoneUpdate getZoneUpdateMsg();
  void setOnMapUpdateReceived(const std::function<void(MapUpdate&)>& func) {
    on_map_update_received_ = func;
  };
  void setOnZoneUpdateReceived(const std::function<void(ZoneUpdate&)>& func) {
    on_zone_update_received_ = func;
  };
  void setOnSSHTokenReceived(const std::function<void(Json&)>& func) {
    on_ssh_token_received_ = func;
  };

 private:
  std::string getTopicFromString(std::string topic_name);
  NetworkConfiguration config_;
  BaseSubscribedTopic<MapUpdate> rx_map_update_;
  BaseSubscribedTopic<ZoneUpdate> rx_zone_update_;
  BaseSubscribedTopic<Json> rx_ssh_token_;

  std::function<void(MapUpdate&)> on_map_update_received_;
  std::function<void(ZoneUpdate&)> on_zone_update_received_;
  std::function<void(Json&)> on_ssh_token_received_;
  void registerSubscribers();
};
}  // namespace vda5050
}  // namespace iw