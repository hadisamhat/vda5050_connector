#pragma once
#include <memory>

#include "iw_logging/logger_base.hpp"
#include "nlohmann/json.hpp"

namespace vda5050_connector {
namespace interface {

class BaseInterface {
 public:
  std::shared_ptr<iw::logging::LoggerBase> logger_;
  virtual nlohmann::json to_json() = 0;
  virtual void from_json(const nlohmann::json& j) = 0;
};
}  // namespace interface
}  // namespace vda5050_connector
