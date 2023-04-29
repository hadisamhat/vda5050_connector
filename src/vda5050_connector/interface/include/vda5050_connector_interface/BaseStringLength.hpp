#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseStringLength : public BaseInterface {
 public:
  int msgLen;
  int topicSerialLen;
  int topicElemLen;
  int idLen;
  bool idNumericalOnly;
  int enumLen;
  int loadIdLen;
};

}  // namespace interface
}  // namespace vda5050_connector
