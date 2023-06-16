#pragma once
#include "vda5050_connector_interface/BaseTopicInterface.hpp"
namespace vda5050_connector {
namespace interface {

template <class HeaderT, class TypeSpecificationT, class PhysicalParametersT, class ProtocolLimitsT,
    class ProtocolFeaturesT, class AgvGeometryT, class LoadSpecificationT>
class BaseFactSheet : public BaseTopicInterface<HeaderT> {
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