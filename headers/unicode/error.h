
#pragma once

#include <stdexcept>

namespace Unicode {

class Error : public std::runtime_error {
protected:
  Error(const std::string& message)
    : std::runtime_error(Namespace("Unicode", message)) {}

  static std::string Namespace(
    const std::string &prefix,
    const std::string &message
  ) {
    return prefix + "::" + message;
  }
  static std::string Message(
    const std::string &prefix,
    const std::string &message
  ) {
    return prefix + ": " + message;
  }
}; // class Error
}; // namespace Unicode
