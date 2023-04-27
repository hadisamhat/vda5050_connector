#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class PolygonPointT>
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
