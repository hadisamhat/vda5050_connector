#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {
class BaseProtocolHeader : public BaseInterface {
 public:
  int headerId;
  std::string timestamp;
  std::string version;
  std::string manufacturer;
  std::string serialNumber;
};
}  // namespace interface
}  // namespace vda5050_connector
