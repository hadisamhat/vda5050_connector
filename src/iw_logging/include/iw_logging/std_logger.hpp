#pragma once

#include <iostream>
#include <string>

#include "iw_logging/logger_base.hpp"

namespace iw {
namespace logging {

/// Logging class using std::cout, should be used only for middleware independent testing
class StdLogger : public LoggerBase {
 public:
  /// Constructor
  StdLogger() = default;
  /// Destructor
  ~StdLogger() = default;
  // Logs message with severity level Debug
  void logDebug(const std::string& msg) override {
    std::cout << "[Debug]: " << msg << std::endl << std::flush;
  };
  /// Logs message with severity level Debug
  void logDebug(const std::stringstream& msg) override {
    std::cout << "[Debug]: " << msg.str() << std::endl << std::flush;
  };
  /// Logs message with severity level Info
  void logInfo(const std::string& msg) override {
    std::cout << "[Info]: " << msg << std::endl << std::flush;
  };
  /// Logs message with severity level Info
  void logInfo(const std::stringstream& msg) override {
    std::cout << "[Info]: " << msg.str() << std::endl << std::flush;
  };
  /// Logs message with severity level Warn
  void logWarn(const std::string& msg) override {
    std::cout << "[Warn]: " << msg << std::endl << std::flush;
  };
  /// Logs message with severity level Warn
  void logWarn(const std::stringstream& msg) override {
    std::cout << "[Warn]: " << msg.str() << std::endl << std::flush;
  };
  /// Logs message with severity level Error
  void logError(const std::string& msg) override {
    std::cout << "[Error]: " << msg << std::endl << std::flush;
  };
  /// Logs message with severity level Error
  void logError(const std::stringstream& msg) override {
    std::cout << "[Error]: " << msg.str() << std::endl << std::flush;
  };
};
}  // namespace logging
}  // namespace iw
