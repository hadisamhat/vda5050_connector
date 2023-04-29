#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseInfoReference : public BaseInterface {
 public:
  std::string referenceKey;
  std::string referenceValue;
};

}  // namespace interface
}  // namespace vda5050_connector
