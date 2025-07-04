
#pragma once

#include <stdexcept>
#include <format>

namespace Unicode {

class Error : public std::runtime_error {
protected:
  Error(const std::string& message)
    : std::runtime_error(Namespace("Unicode", message)) {}

  static std::string Namespace(
    const std::string &prefix,
    const std::string &message
  ) {
    return std::format("{}::{}", prefix, message);
  }
  static std::string Message(
    const std::string &prefix,
    const std::string &message
  ) {
    return std::format("{}: {}", prefix, message);
  }
}; // class Error
}; // namespace Unicode
