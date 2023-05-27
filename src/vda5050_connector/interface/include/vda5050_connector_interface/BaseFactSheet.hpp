#pragma once
#if __cplusplus < 201703L
#include <experimental/optional>
namespace std {
namespace experimental {}
using namespace experimental;
}  // namespace std
#else
#include <optional>
#endif
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