#pragma once
#if __cplusplus < 201703L
#include <experimental/filesystem>
namespace std {
using filesystem = experimental::filesystem
}
#else
#include <filesystem>
#endif

#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

namespace OrientationType {
constexpr char GLOBAL[] = "GLOBAL";
constexpr char TANGENTIAL[] = "TANGENTIAL";

}  // namespace OrientationType

template <class TrajectoryT, class ActionT>
class BaseEdge : public BaseInterface {
 public:
  std::string edgeId;
  int sequenceId;
  std::string edgeDescription;  // OPTIONAL
  bool released;
  std::string startNodeId;
  std::string endNodeId;
  float maxSpeed;     // OPTIONAL, Unit : meters/second
  float maxHeight;    // OPTIONAL, Unit : meters
  float minHeight;    // OPTIONAL, Unit : meters
  float orientation;  // OPTIONAL, Unit : radians
  std::string orientationType;
  std::string direction;   // OPTIONAL
  bool rotationAllowed;    // OPTIONAL
  float maxRotationSpeed;  // OPTIONAL, Unit : radians/second
  TrajectoryT traj;        // OPTIONAL
  float length;            // OPTIONAL, Unit : meters
  std::vector<ActionT> actions;

  virtual std::optional<ActionT> tryGetActionOfType(
      const std::string& action_type) = 0;
  virtual bool hasActionOfType(const std::string& action_type) const = 0;
};

}  // namespace interface
}  // namespace vda5050_connector
