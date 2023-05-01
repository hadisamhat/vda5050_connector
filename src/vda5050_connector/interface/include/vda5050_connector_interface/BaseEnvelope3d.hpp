#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseEnvelope3d : public BaseInterface {
 public:
  std::string set;
  std::string format;
  nlohmann::json data;
  std::string url;
  std::string description;
};
}  // namespace interface
}  // namespace vda5050_connector
