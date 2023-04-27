#pragma once
#include <memory>

#include "vda5050_connector/interface/BaseInterface.hpp"

namespace iw {
namespace vda5050_connector_interface {
template <class HeaderT>
class BaseTopicInterface : public BaseInterface {
 public:
  std::shared_ptr<HeaderT> header;
};
}  // namespace vda5050_connector_interface
}  // namespace iw
