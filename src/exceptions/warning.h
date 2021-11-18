#ifndef GRAPH3D_EXCEPTIONS_WARNING_H
#define GRAPH3D_EXCEPTIONS_WARNING_H

#include <iostream>
#include <sstream>

namespace graph3d {
namespace exceptions {
class warning {
  std::string message, description;
  const char *errCode;

 public:
  warning(const char* errCode, const std::string description, const std::string message = std::string())
      : errCode(errCode), description(description), message(message) {
    print();
  }

  virtual void print() const {
    std::clog << "/* ** WARNING ** */\n"
              << "Code: " << errCode << '\n'
              << "Error: " << description << '\n'
              << (message.empty() ? "" : message + '\n')
              << " -- --------------------------------------------------- -- \n";
  }
};
}  // namespace exceptions
}  // namespace graph3d

#endif