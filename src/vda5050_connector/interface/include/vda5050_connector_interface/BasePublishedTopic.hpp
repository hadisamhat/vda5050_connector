#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include "vda5050_connector_interface/BaseNetworkConfiguration.hpp"

namespace vda5050_connector {
namespace interface {

template <class T>
class BasePublishedTopic {
 public:
  std::string topic_name;
  std::mutex pub_mutex;
  double update_time_s;
  bool enable{true};
  T msg;
};

}  // namespace interface
}  // namespace vda5050_connector