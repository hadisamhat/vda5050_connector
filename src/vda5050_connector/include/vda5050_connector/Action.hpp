#pragma once
#include "vda5050_connector/ActionParameter.hpp"
#include "vda5050_connector_interface/BaseAction.hpp"

namespace vda5050_connector {
namespace impl {

namespace ActionParamConsts {
constexpr char X[] = "x";
constexpr char Y[] = "y";
constexpr char THETA[] = "theta";
constexpr char WAIT_FOR[] = "waitfor";
constexpr char TRIGGER_EVENT[] = "event";
constexpr char CHARGE_THRESHOLD[] = "threshold";
}  // namespace ActionParamConsts

// The actions performed by the STR, they are in lowercase in order to have the processing be case
// insensitive.
namespace strActions {
constexpr char PAUSE[] = "startpause";
constexpr char UNPAUSE[] = "stoppause";
constexpr char START_CHARGE[] = "startcharging";
constexpr char STOP_CHARGE[] = "stopcharging";
constexpr char STATE_REQUEST[] = "staterequest";
constexpr char FINE_POSITIONING[] = "finePositioning";
constexpr char PICK[] = "pick";
constexpr char DROP[] = "drop";
constexpr char INIT_POSITION[] = "initposition";
constexpr char CANCEL_ORDER[] = "cancelorder";
constexpr char LOG_REPORT[] = "logreport";
constexpr char DETECT_OBJECT[] = "detectObject";
constexpr char WAIT_FOR_TRIGGER[] = "waitfortrigger";
constexpr char TRIGGER[] = "triggerevent";
constexpr char FACTSHEET_REQUEST[] = "factsheetRequest";
}  // namespace strActions

// Map containing the action parameters required for each action.
const std::map<std::string, std::vector<std::string>> actionParametersPerAction = {
    {strActions::TRIGGER, {ActionParamConsts::TRIGGER_EVENT}},
    {strActions::WAIT_FOR_TRIGGER, {ActionParamConsts::WAIT_FOR}},
    {strActions::INIT_POSITION,
        {ActionParamConsts::X, ActionParamConsts::Y, ActionParamConsts::THETA}}};

// Vector containing all actions allowed on edges.
const std::vector<std::string> allowedNodeActions = {strActions::START_CHARGE,
    strActions::STOP_CHARGE, strActions::PICK, strActions::DROP, strActions::INIT_POSITION,
    strActions::WAIT_FOR_TRIGGER, strActions::WAIT_FOR_TRIGGER, strActions::TRIGGER};

// Vector containing all actions allowed on edges.
const std::vector<std::string> allowedEdgeActions = {
    strActions::PICK, strActions::DROP, strActions::TRIGGER};

// Vector containing all actions allowed as instantActions.
const std::vector<std::string> allowedInstantActions = {strActions::START_CHARGE,
    strActions::STOP_CHARGE, strActions::CANCEL_ORDER, strActions::STATE_REQUEST,
    strActions::LOG_REPORT, strActions::PAUSE, strActions::UNPAUSE, strActions::INIT_POSITION,
    strActions::TRIGGER};

class Action : public interface::BaseAction<ActionParameter> {
 public:
  nlohmann::json to_json() override;
  void from_json(const nlohmann::json& j) override;
  bool operator==(const Action& a) const;
  bool requiresActionParameters() const override;
  bool hasValidActionParameters() const override;
  bool isSupportedAction(const std::vector<std::string>& allowedActions) const override;

  template <typename T>
  std::optional<T> getParameterByKey(const char* key) const {
    auto it = find_if(this->actionParameters.begin(), this->actionParameters.end(),
        [key](const ActionParameter& ap) { return ap.key == key; });
    if (it != this->actionParameters.end()) {
      std::istringstream ss(it->value);
      T value;

      // If the type requested is string or json, then parse using nlohmann json library.
      if (std::is_same<T, std::string>::value) {
        value = nlohmann::json(ss.str()).get<T>();
      } else if (std::is_same<T, nlohmann::json>::value) {
        value = nlohmann::json::parse(ss.str(), nullptr, false);
      } else {
        ss >> value;
      }

      return value;
    }
    return std::nullopt;
  }

 private:
  void UpperFirst(std::string& s);
};
}  // namespace impl
}  // namespace vda5050_connector