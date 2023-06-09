#pragma once
#include <string>

#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

namespace ConnectionStates {
// The connection between AGV and broker is active.
constexpr char ONLINE[] = "ONLINE";

// The connection between AGV and broker has gone offline in a coordinated way.
constexpr char OFFLINE[] = "OFFLINE";

// The connection between AGV and broker has unexpectedly ended.
constexpr char CONNECTIONBROKEN[] = "CONNECTIONBROKEN";

};  // namespace ConnectionStates

class BaseConnection : public BaseInterface {
 public:
  std::string connectionState;
};
}  // namespace interface
}  // namespace vda5050_connector