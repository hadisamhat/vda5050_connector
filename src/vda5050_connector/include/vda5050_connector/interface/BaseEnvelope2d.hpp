#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class PolygonPointT>
class BaseEnvelope2d : public BaseInterface {
 public:
  std::string set;
  std::vector<PolygonPointT> polygonPoints;
  std::string description;
};
}  // namespace interface
}  // namespace vda5050_connector
