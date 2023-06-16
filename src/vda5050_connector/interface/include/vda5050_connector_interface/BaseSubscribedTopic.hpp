#include <memory>
#include <mutex>

namespace vda5050_connector {
namespace interface {

template <class T>
class BaseSubscribedTopic {
 public:
  std::string topic_name;
  std::mutex sub_mutex;
  T msg;
};

}  // namespace interface
}  // namespace vda5050_connector