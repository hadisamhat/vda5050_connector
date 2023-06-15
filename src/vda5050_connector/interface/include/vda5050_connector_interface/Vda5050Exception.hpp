#pragma once
#include <exception>
#include <iostream>

namespace vda5050 {
namespace exception {
// Custom exception class derived from std::exception
class Vda5050Exception : public std::exception {
 public:
  Vda5050Exception(const char* message) : errorMessage(message) {}

  // Override the what() function to provide custom error message
  const char* what() const throw() { return errorMessage.c_str(); }

 private:
  std::string errorMessage;
};
}  // namespace error
}  // namespace vda5050
