#pragma once
#include "vda5050_connector/interface/BaseInterface.hpp"

namespace vda5050_connector {
namespace interface {

namespace ErrorLevel {
constexpr char WARNING[] = "warning";
constexpr char FATAL[] = "fatal";
};  // namespace ErrorLevel

namespace ErrorType {
constexpr char VALIDATION[] = "validationError";
constexpr char ORDER[] = "orderError";
constexpr char UPDATE[] = "orderUpdateError";
constexpr char ROUTE[] = "noRouteError";
constexpr char NOORDER[] = "noOrderToCancel";
constexpr char EXECUTION[] = "executionError";
}  // namespace ErrorType

template <class ErrorReferenceT>
class BaseError : public BaseInterface {
 public:
  std::string errorType;
  std::vector<ErrorReferenceT> errorReferences;  // OPTIONAL
  std::string errorDescription;                  // OPTIONAL
  std::string errorLevel;
  std::string errorHint;  // NOT PART OF VDA STANDARD
};

}  // namespace interface
}  // namespace vda5050_connector
