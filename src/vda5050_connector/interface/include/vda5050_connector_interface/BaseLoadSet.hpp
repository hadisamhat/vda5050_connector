#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

template <class BoundingBoxReferenceT, class LoadDimensionsT>
class BaseLoadSet : public BaseInterface {
 public:
  std::string setName;
  std::string loadType;
  std::string description;
  std::vector<std::string> loadPositions;
  BoundingBoxReferenceT boundingBoxReference;
  LoadDimensionsT loadDimensions;
  float maxWeight;
  float minLoadhandlingHeight;
  float maxLoadhandlingHeight;
  float minLoadhandlingDepth;
  float maxLoadhandlingDepth;
  float minLoadhandlingTilt;
  float maxLoadhandlingTilt;
  float agvSpeedLimit;
  float agvDecelerationLimit;
  float agvAccelerationLimit;
  float pickTime;
  float dropTime;
};

}  // namespace interface
}  // namespace vda5050_connector
