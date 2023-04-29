#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseTimingInfo : public BaseInterface {
 public:
  float minOrderInterval;
  float minStateInterval;
  float defaultStateInterval;
  float visualizationInterval;
};

}  // namespace interface
}  // namespace vda5050_connector
