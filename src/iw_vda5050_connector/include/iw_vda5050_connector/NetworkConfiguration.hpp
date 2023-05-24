#pragma once

#include <memory>
#include <string>
#include <vector>
#include <regex>

#include "vda5050_connector_interface/BaseNetworkConfiguration.hpp"

constexpr char DEFAULT_SSH_TUNNEL_TOPIC[] = "$aws/things/CLIENT_ID/tunnels/notify";

namespace iw {
namespace vda5050 {

class NetworkConfiguration : public vda5050_connector::interface::BaseNetworkConfiguration {
 public:
  std::string map_update_topic_name;
  std::string zone_update_topic_name;
  std::string ssh_topic_name;
};

}  // namespace vda5050
}  // namespace iw