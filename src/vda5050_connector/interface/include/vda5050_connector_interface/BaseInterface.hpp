#pragma once
#include <memory>

#if __cplusplus < 201703L
#include <experimental/optional>
namespace std {
template <typename T>
using optional = experimental::optional<T>;
using experimental::nullopt;
}  // namespace std
#else
#include <optional>
#endif

#include "iw_logging/logger_base.hpp"
#include "iw_logging/std_logger.hpp"
#include "nlohmann/json.hpp"

namespace vda5050_connector {
namespace interface {

class BaseInterface {
 public:
  virtual nlohmann::json to_json() = 0;
  virtual void from_json(const nlohmann::json& j) = 0;
};
}  // namespace interface
}  // namespace vda5050_connector
