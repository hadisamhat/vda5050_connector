#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

namespace SupportType {
constexpr char SUPPORTED[] = "SUPPORTED";
constexpr char REQUIRED[] = "REQUIRED";
};  // namespace SupportType

class BaseOptionalParameter : public BaseInterface {
 public:
  std::string parameter;
  std::string support;
  std::string description;
};

}  // namespace interface
}  // namespace vda5050_connector
