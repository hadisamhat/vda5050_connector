#include "vda5050_connector/ManagerFSM.hpp"

using namespace vda5050_connector::interface;
using namespace Aws::Crt;
using namespace std::chrono;

namespace vda5050_connector {
namespace impl {

void ManagerFSM::initializeTransitions() {
  state_machine_->addTransition(
      FSMState::INIT, FSMState::CONNECTED, [this] { return tls_initialized_; }, [this] {});
  state_machine_->addTransition(
      FSMState::CONNECTED, FSMState::OPERATIONAL,
      [this] { return !error_ && connection_completed_promise_.get_future().get(); }, [this] {});
  //   state_machine_->addTransition(
  //       FSMState::OPERATIONAL, FSMState::DISCONNECTED,
  //       [this] { return !error_ && connection_completed_promise_.get_future().get(); }, [this]
  //       {});
  for (const auto& state_pair : state_to_str_) {
    const auto state = state_pair.first;
    if (state != FSMState::ERROR) {
      state_machine_->addTransition(
          state, FSMState::ERROR, [this] { return error_; }, [this] {});
    }
  }
}
}  // namespace impl
}  // namespace vda5050_connector