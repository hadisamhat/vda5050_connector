#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

class BaseTypeSpecification : public BaseInterface {
 public:
  std::string seriesName;
  std::string seriesDescription;
  std::string agvKinematic;
  std::string agvClass;
  float maxLoadMass;
  std::vector<std::string> localizationTypes;
  std::vector<std::string> navigationTypes;
};

}  // namespace interface
}  // namespace vda5050_connector
