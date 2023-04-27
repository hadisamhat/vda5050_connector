#include "iw_logging/directory_logger.hpp"

using namespace std;

namespace iw {
namespace logging {

DirectoryLogger::DirectoryLogger(fs::path log_dir, size_t queue_size)
    : log_dir_(log_dir), max_queue_size_(queue_size) {
  if (!fs::exists(log_dir_))
    throw std::runtime_error(
        log_dir_.string() + " does not exist, unable to create DirectoryLogger");

  // list all the files in the directory
  for (auto& entry : fs::directory_iterator(log_dir_)) {
    queue_.push_back(entry.path());
  }
  std::sort(queue_.begin(), queue_.end());
  // Remove the first file until the size is <= queue_size
  while (queue_.size() > queue_size) {
    fs::remove_all(queue_.front());
    queue_.pop_front();
  }
}

std::string DirectoryLogger::getCurrentTime() {
  stringstream ss;
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
  return ss.str();
}

fs::path DirectoryLogger::createDirectory() {
  fs::path dir_path = log_dir_ / this->getCurrentTime();
  fs::create_directory(dir_path);
  fs::permissions(dir_path, fs::perms::owner_all | fs::perms::group_exec | fs::perms::group_read |
                                fs::perms::others_exec | fs::perms::others_read);
  if (queue_.empty() || (dir_path != queue_.back())) queue_.push_back(dir_path);
  if (queue_.size() > max_queue_size_) {
    fs::remove_all(queue_.front());
    queue_.pop_front();
  }
  return dir_path;
}

fs::path DirectoryLogger::getLatestDirectory() { return queue_.back(); }

void DirectoryLogger::clear() {
  fs::remove_all(log_dir_);
  fs::create_directory(log_dir_);
}

}  // namespace logging
}  // namespace iw