
#pragma once

#include <cstdint>
#include <unicode/type.h>
#include <unicode/length.h>
#include <unicode/error.h>

namespace Unicode {
//
struct LengthError : Error {
  LengthError(const std::string &message)
    : Error(Message("LengthError", message)) {}
};

template <typename Char>
std::uint64_t FindLength(const BasicString<Char> &input) {
  static_assert(
    std::is_same_v<Char, Char8>  ||
    std::is_same_v<Char, Char16> ||
    std::is_same_v<Char, Char32>,
    "Undefined character type"
  );
  return 0;
}
template <>
std::uint64_t FindLength(const StringU8 &input);

template <>
std::uint64_t FindLength(const StringU16 &input);

template <>
std::uint64_t FindLength(const StringU32 &input);

}
