#pragma once
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {
namespace EStopType {
constexpr char AUTOACK[] = "AUTOACK";
constexpr char MANUAL[] = "MANUAL";
constexpr char REMOTE[] = "REMOTE";
constexpr char NONE[] = "NONE";
};  // namespace EStopType
class BaseSafetyState : public BaseInterface {
 public:
  std::string eStop;
  bool fieldViolation;
};

}  // namespace interface
}  // namespace vda5050_connector
