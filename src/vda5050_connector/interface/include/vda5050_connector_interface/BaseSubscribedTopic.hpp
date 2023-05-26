#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "vda5050_connector_interface/BaseInterface.hpp"

using Json = nlohmann::json;

namespace vda5050_connector {
namespace interface {

template <class T>
class BaseSubscribedTopic{
 public:
  std::string topic_name;
  std::mutex sub_mutex;
  T msg;
};

}  // namespace interface
}  // namespace vda5050_connector