#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class StringLengthT, class TimingInfoT, class ArrayLengthT>
class BaseProtocolLimits : public BaseInterface {
 public:
  StringLengthT maxStringLens;
  ArrayLengthT maxArrayLens;
  TimingInfoT timing;
};

}  // namespace interface
}  // namespace vda5050_connector
