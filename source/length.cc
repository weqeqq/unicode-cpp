
#include <cstdint>
#include <unicode/length.h>

#ifdef UNICODE_USE_SIMDUTF
#include <simdutf.h>
#endif // UNICODE_USE_SIMDUTF 

namespace Unicode {

#ifdef UNICODE_USE_SIMDUTF
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
#else
template <>
std::uint64_t FindLength(const StringU8 &input) {
  std::uint64_t length = 0;

  for (auto character : input) {
    if ((character & 0xC0) != 0x80) { 
      length++;
    }
  }
  return length;
}
template <>
std::uint64_t FindLength(const StringU16 &input) {
  std::uint64_t length = 0;

  for (auto iterator = input.begin(); iterator != input.end(); iterator++) {
    if (static_cast<std::uint16_t>(*iterator) >= 0xD800 &&
        static_cast<std::uint16_t>(*iterator) <= 0xDBFF) {
      iterator++;
      if (static_cast<std::uint16_t>(*iterator) >= 0xDC00 &&
          static_cast<std::uint16_t>(*iterator) <= 0xDFFF) {
        length++;
      } else {
        throw LengthError::FailedToFindLength("Invalid UTF-16 string");
      }
    } else {
      length++;
    }
  }
  return length;
}
template <>
std::uint64_t FindLength(const StringU32 &input) {
  return input.size();
}
#endif // UNICODE_USE_SIMDUTF
}
