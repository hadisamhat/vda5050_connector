#pragma once
#include "vda5050_connector/BoundingBoxReference.hpp"
#include "vda5050_connector/LoadDimension.hpp"
#include "vda5050_connector_interface/BaseInterface.hpp"

namespace iw {
namespace vda5050 {
class Load : public vda5050_connector::interface::BaseInterface {
 public:
  std::string loadId;                                         // OPTIONAL
  std::string loadType;                                       // OPTIONAL
  std::string loadPosition;                                   // OPTIONAL
  vda5050_connector::impl::BoundingBoxReference bbReference;  // OPTIONAL
  vda5050_connector::impl::LoadDimension dimensions;          // OPTIONAL
  float weight = 0;
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
};
}  // namespace vda5050
}  // namespace iw
