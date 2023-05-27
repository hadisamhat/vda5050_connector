#pragma once

#include <algorithm>
#include <deque>
#include <exception>
#if __has_include(<filesystem>)
#include <filesystem>
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace std {
namespace filesystem = std::experimental::filesystem;
}
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