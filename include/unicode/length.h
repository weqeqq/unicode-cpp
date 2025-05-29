
#pragma once 

#include <cstdint>
#include <unicode/type.h>
#include <unicode/length.h>
#include <unicode/error.h>

namespace Unicode {

EXCEPTION_DEFINE_NAMESPACE(
  LengthError, 
  EXCEPTION_ROOT,
  EXCEPTION_MESSAGE(FailedToFindLength);
);
EXCEPTION_DEFINE_MESSAGE(
  FailedToFindLength, 
  LengthError
);

template <typename Char>
std::uint64_t FindLength(const BasicString<Char> &input) {
  static_assert(false, "Undefined character type");

  return 0;
}
template <>
std::uint64_t FindLength(const StringU8 &input);

template <>
std::uint64_t FindLength(const StringU16 &input);

template <>
std::uint64_t FindLength(const StringU32 &input);

}
