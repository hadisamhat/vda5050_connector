#pragma once

#include <functional>
#include <memory>
#include <mutex>

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
  void updatePublisher(const std::function<void(T&)>& func) {
    std::lock_guard<std::mutex> lock(pub_mutex);
    func(msg);
  };
};

}  // namespace interface
}  // namespace vda5050_connector