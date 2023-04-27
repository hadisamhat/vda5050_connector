#include <iostream>

#include "vda5050_connector_interface/BaseInterface.hpp"

using namespace iw::vda5050_connector_interface;
class BaseloadInterface : public BaseInterface {
 public:

  float x, y, z;
  nlohmann::json to_json() override  {
    nlohmann::json b;
    b["b"] = 1;
    return b;
  }
  std::string test() {
    return to_json().dump(2);
  };
  void from_json(const nlohmann::json& j) {}
};

class Baseload : public BaseloadInterface {
 public:
  int test;
  nlohmann::json to_json() override {
    nlohmann::json b;
    b["a"] = 1;
    return b;
  }
  void from_json(const nlohmann::json& j) {}
};

class BaseSetInterface : public BaseInterface {
 public:
  std::shared_ptr<BaseloadInterface> loadInterface;
  nlohmann::json to_json() override  {
    nlohmann::json b;
    b["a"] = loadInterface->test();
    return b;
  }
  void from_json(const nlohmann::json& j) {}
};

class Baseload2 {
 public:
  std::shared_ptr<BaseloadInterface> loadInterface;
  void from_json(const nlohmann::json& j) {}
};

// class HeaderTest : public BaseProtocolHeader<HeaderTest> {
//   public:
//     int x;
//   void to_json(nlohmann::json& j, const HeaderTest& b) override { std::cout << "sdas"; }
//   void from_json(const nlohmann::json& j, HeaderTest& b) override { std::cout << "sdas"; };
// };
// class TopicTest : public BaseTopicInterface<TopicTest, HeaderTest> {
//   void to_json(nlohmann::json& j, const TopicTest& b) override {}
//   void from_json(const nlohmann::json& j, TopicTest& b) override {}
// };

// class ActionParameterTest : public BaseActionParameter<ActionParameterTest> {};
// class Action : public BaseAction<ActionParameterTest> {};
int main() {
  BaseSetInterface interface;
  auto ov = std::make_shared<Baseload>();
  interface.loadInterface = ov;
  std::cout << interface->() << std::endl;
}