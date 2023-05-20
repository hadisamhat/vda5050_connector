#pragma once

#include <memory>
#include <string>
#include <vector>

class MQTTConfiguration {
 public:
  std::string cert_path;
  std::string priv_key_path;
  std::string root_ca_path;
  std::string client_id_path;
  std::string endpoint;
  int max_reconnect_attempts;
  int min_reconnect_backoff_sec;
  int max_reconnect_backoff_sec;
  std::string protocol_version;
  std::string manufacturer;
  std::string serial_number;
  int connection_state_interval_secs;
  int state_interval_secs;
  int visualization_interval_secs;
  int factsheet_interval_secs;
  bool mute_coms{false};
  std::string mode;
  std::string dev_topic_prefix;
  std::string qa_topic_prefix;
};
