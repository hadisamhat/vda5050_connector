#pragma once

#include <memory>
#include <string>
#include <vector>

namespace vda5050_connector {
namespace interface {

class BaseNetworkConfiguration {
 public:
  std::string cert_path;
  std::string priv_key_path;
  std::string root_ca_path;
  std::string client_id_path;
  std::string endpoint;
  std::string protocol_version;
  std::string manufacturer;
  std::string serial_number;
  std::string mode;
  std::string dev_topic_prefix;
  std::string qa_topic_prefix;

  std::string instant_action_topic_name;
  std::string order_topic_name;
  std::string visualization_topic_name;
  std::string connection_topic_name;
  std::string fact_sheet_topic_name;
  std::string state_topic_name;

  int max_reconnect_attempts;
  int min_reconnect_backoff_sec;
  int max_reconnect_backoff_sec;

  int connection_state_interval_secs;
  int state_interval_secs;
  int visualization_interval_secs;
  int fact_sheet_interval_secs;
  bool mute_coms{false};
};

}  // namespace interface
}  // namespace vda5050_connector