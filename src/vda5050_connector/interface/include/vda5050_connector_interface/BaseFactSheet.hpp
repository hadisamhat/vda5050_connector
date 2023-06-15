#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class TypeSpecificationT, class PhysicalParametersT, class ProtocolLimitsT,
    class ProtocolFeaturesT, class AgvGeometryT, class LoadSpecificationT>
class BaseFactSheet : public BaseInterface {
 public:
  TypeSpecificationT typeSpecification;
  PhysicalParametersT physicalParameters;
  ProtocolLimitsT protocolLimits;
  ProtocolFeaturesT protocolFeatures;
  AgvGeometryT agvGeometry;
  LoadSpecificationT loadSpecification;
};
}  // namespace interface
}  // namespace vda5050_connector