#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "vda5050_connector/ProtocolHeader.hpp"
#include "vda5050_connector_interface/BaseTopicInterface.hpp"

using Json = nlohmann::json;
using namespace vda5050_connector::impl;

namespace vda5050_connector {
namespace interface {

template <class T>
class BasePublishedTopic : public BaseTopicInterface<ProtocolHeader> {
 public:
  std::string topic_name;
  std::mutex pub_mutex;
  double update_time_s;
  bool enable{true};
  T msg;

  Json to_json() override {
    Json j;
    j.merge_patch(this->header.to_json());
    j.merge_patch(msg.to_json());
    return j;
  };

  void from_json(const Json& j) override {
    this->header.from_json(j);
    msg.from_json(j);
  }
};

}  // namespace interface
}  // namespace vda5050_connector