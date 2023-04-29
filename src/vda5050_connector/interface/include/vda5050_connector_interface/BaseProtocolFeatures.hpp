#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class OptionalParameterT, class AgvActionT>
class BaseProtocolFeatures : public BaseInterface {
 public:
  std::vector<OptionalParameterT> optionalParameters;
  std::vector<AgvActionT> agvActions;
  std::string resultDescription;
};

}  // namespace interface
}  // namespace vda5050_connector
