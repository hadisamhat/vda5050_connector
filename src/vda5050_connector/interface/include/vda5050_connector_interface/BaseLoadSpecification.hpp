#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class LoadSetT>
class BaseLoadSpecification : public BaseInterface {
 public:
  std::vector<std::string> loadPositions;
  LoadSet boundingBoxReference;
};

}  // namespace interface
}  // namespace vda5050_connector
