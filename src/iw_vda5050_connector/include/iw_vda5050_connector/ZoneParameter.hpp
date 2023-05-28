#include <boost/algorithm/string.hpp>
#include <experimental/optional>

#include "vda5050_connector_interface/BaseInterface.hpp"

namespace iw {
namespace vda5050 {

class ZoneParameter : public vda5050_connector::interface::BaseInterface {
 public:
  std::string key;
  std::string value;
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;

 private:
  void UpperFirst(std::string& s);
};
}  // namespace vda5050
}  // namespace iw