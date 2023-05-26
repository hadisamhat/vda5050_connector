#pragma once

#include <algorithm>
#include <deque>
#include <exception>
#if __cplusplus < 201703L
#include <experimental/filesystem>
namespace std {
using filesystem = experimental::filesystem;
}
#else
#include <filesystem>
#endif
#include <iomanip>
#include <sstream>
#include <string>

namespace iw {
namespace logging {

namespace fs = std::filesystem;

/// Implementation of a cyclic directory logging helper class
class DirectoryLogger {
 public:
  /**
   * @brief Construct a new Directory Logger object
   *
   * @param log_dir Directory to log to
   * @param queue_size size of the queue of folders
   */
  DirectoryLogger(fs::path log_dir, size_t queue_size);
  /// Create a directory and return it's path
  fs::path createDirectory();
  /// Get latest created dir
  fs::path getLatestDirectory();
  /// Remove all dirs
  void clear();

 private:
  std::deque<std::string> queue_;
  fs::path log_dir_;
  size_t max_queue_size_;
  std::string getCurrentTime();
};

}  // namespace logging
}  // namespace iw