#ifndef GRAPH3D_EXCEPTIONS_EXCEPTION_H
#define GRAPH3D_EXCEPTIONS_EXCEPTION_H

#include <iostream>
#include <sstream>

namespace graph3d {
namespace exceptions {
class exception : public std::exception {
  std::string message, description;
  const char *errCode;

 public:
  exception(const char* errCode, const std::string description, const std::string message = std::string())
      : errCode(errCode), description(description), message(message) {}

  virtual void print() const {
    std::cerr << "/* ** Exception Ocurred ** */\n"
              << "Code: " << errCode << '\n'
              << "Error: " << description << '\n'
              << (message.empty() ? "" : message + '\n')
              << " -- --------------------------------------- -- \n";
  }

  virtual char const* what() const noexcept {
    return "/* ** 3DGraph Custom Exception Ocurred ** */";
  }
};
}  // namespace exceptions
}  // namespace graph3d

#endif