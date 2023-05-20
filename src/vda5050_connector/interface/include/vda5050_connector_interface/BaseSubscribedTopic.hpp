#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "vda5050_connector_interface/BaseNetworkConfiguration.hpp"

namespace vda5050_connector {
namespace interface {

template <class T>
class BaseSubscribedTopic {
 public:
  std::string topic_name;
  std::mutex sub_mutex;
  T msg;
  std::function<void(const T&)> on_receive_fct;

  void setOnReceive(const std::function<void(const T&)>& func) { on_receive_fct = func; };
  void onReceive() { on_receive_fct(msg); };
};

}  // namespace interface
}  // namespace vda5050_connector