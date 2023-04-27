#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {
namespace InfoLevel {
constexpr char DEBUG[] = "debug";
constexpr char INFO[] = "info";
};  // namespace InfoLevel

template <class InfoReferenceT>
class BaseInfo : public BaseInterface {
 public:
  std::string infoType;
  std::vector<InfoReferenceT> infoReferences;  // OPTIONAL
  std::string infoDescription;                 // OPTIONAL
  std::string infoLevel;
};

}  // namespace interface
}  // namespace vda5050_connector
