#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {
class BaseActionParameter : public BaseInterface {
 public:
  std::string key;
  std::string value;
  std::string description;
  bool isOptional{false};
};
}  // namespace interface
}  // namespace vda5050_connector
