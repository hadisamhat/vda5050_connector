#pragma once
#include <memory>

#include "vda5050_connector_interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {
template <class HeaderT>
class BaseTopicInterface : public BaseInterface {
 public:
  HeaderT header;
};
}  // namespace interface
}  // namespace vda5050_connector
