#pragma once
#include <memory>

#include "iw_logging/logger_base.hpp"
#include "iw_logging/std_logger.hpp"
#ifdef __has_include("nlohmann/json.hpp")
#include "nlohmann/json.hpp"
#else
#include "third_party/nlohmann/json.hpp"
#endif
namespace vda5050_connector {
namespace interface {

class BaseInterface {
 public:
  virtual nlohmann::json to_json() = 0;
  virtual void from_json(const nlohmann::json& j) = 0;
};
}  // namespace interface
}  // namespace vda5050_connector
