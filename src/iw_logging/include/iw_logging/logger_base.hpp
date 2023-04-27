#pragma once

#include <sstream>
#include <string>

namespace iw {
namespace logging {

/// Base class for logging
class LoggerBase {
 public:
  /// Destructor, always needs to be overwritten
  virtual ~LoggerBase() = default;
  /// Logs message with severity level Debug
  virtual void logDebug(const std::string& msg) = 0;
  /// Logs message with severity level Debug
  virtual void logDebug(const std::stringstream& msg) = 0;
  /// Logs message with severity level Info
  virtual void logInfo(const std::string& msg) = 0;
  /// Logs message with severity level Info
  virtual void logInfo(const std::stringstream& msg) = 0;
  /// Logs message with severity level Warn
  virtual void logWarn(const std::string& msg) = 0;
  /// Logs message with severity level Warn
  virtual void logWarn(const std::stringstream& msg) = 0;
  /// Logs message with severity level Error
  virtual void logError(const std::string& msg) = 0;
  /// Logs message with severity level Error
  virtual void logError(const std::stringstream& msg) = 0;
};

}  // namespace logging
}  // namespace iw
