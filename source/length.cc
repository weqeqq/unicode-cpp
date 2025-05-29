
#include <unicode/length.h>

#include <simdutf.h>

namespace Unicode {

template <>
std::uint64_t FindLength(const StringU8 &input) {
  auto error = simdutf::validate_utf8_with_errors(input.data(), input.size());
  if (error.is_err()) {
    throw LengthError::FailedToFindLength(
      StringU8(simdutf::error_to_string(error.error))
    );
  }
  return simdutf::count_utf8(input.data(), input.size());
}

template <>
std::uint64_t FindLength(const StringU16 &input) {
  auto error = simdutf::validate_utf16_with_errors(input.data(), input.size());
  if (error.is_err()) {
    throw LengthError::FailedToFindLength(
      StringU8(simdutf::error_to_string(error.error))
    );
  }
  return simdutf::count_utf16(input.data(), input.size());
}

template <>
std::uint64_t FindLength(const StringU32 &input) {
  auto error = simdutf::validate_utf32_with_errors(input.data(), input.size());
  if (error.is_err()) {
    throw LengthError::FailedToFindLength(
      StringU8(simdutf::error_to_string(error.error))
    );
  }
  return input.size();
}
  
}
