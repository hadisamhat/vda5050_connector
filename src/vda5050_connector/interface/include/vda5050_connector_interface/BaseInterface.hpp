#pragma once
#include <memory>

#include "iw_logging/logger_base.hpp"
#include "iw_logging/std_logger.hpp"
#include "nlohmann/json.hpp"

namespace vda5050_connector {
namespace interface {

class BaseInterface {
 public:
  BaseInterface() { logger_ = std::make_shared<iw::logging::StdLogger>(); }
  virtual ~BaseInterface() = default;
  virtual nlohmann::json to_json() = 0;
  virtual void from_json(const nlohmann::json& j) = 0;

 protected:
  std::shared_ptr<iw::logging::LoggerBase> logger_;
};
}  // namespace interface
}  // namespace vda5050_connector
