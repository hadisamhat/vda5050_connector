#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class WheelDefinitionT, class Envelope2dT, class Envelope3dT>
class BaseAgvGeometry : public BaseInterface {
 public:
  WheelDefinitionT maxStringLens;
  Envelope2dT maxArrayLens;
  Envelope3dT timing;
};

}  // namespace interface
}  // namespace vda5050_connector
