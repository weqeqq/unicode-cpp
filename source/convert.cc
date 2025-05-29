
#include <unicode/convert.h>
#include <simdutf.h>

namespace Unicode {

template <>
StringU8 ToStringU8(const StringU8 &input) {
  return input;
}

template <>
StringU8 ToStringU8(const StringU16 &input) {
  if (input.empty()) {
    return StringU8();
  }
  StringU8 output(
    simdutf::utf8_length_from_utf16(input.data(), input.size()), 0
  );
  if (!simdutf::convert_utf16_to_utf8(
    input.data(), input.size(), output.data()
  )) {
    throw ConvertError::FailedToConvert("Invalid UTF-16 string");
  }
  return output;
}

template <>
StringU8 ToStringU8(const StringU32 &input) {
  if (input.empty()) {
    return StringU8();
  }
  StringU8 output(
    simdutf::utf8_length_from_utf32(input.data(), input.size()), 0
  );
  if (!simdutf::convert_utf32_to_utf8(
    input.data(), input.size(), output.data()
  )) {
    throw ConvertError::FailedToConvert("Invalid UTF-32 string");
  }
  return output;
}

template <>
StringU16 ToStringU16(const StringU8 &input) {
  if (input.empty()) {
    return StringU16();
  }
  StringU16 output(
    simdutf::utf16_length_from_utf8(input.data(), input.size()), 0
  );
  if (!simdutf::convert_utf8_to_utf16(
    input.data(), input.size(), output.data()
  )) {
    throw ConvertError::FailedToConvert("Invalid UTF-8 string");
  }
  return output;
}

template <>
StringU16 ToStringU16(const StringU16 &input) {
  return input;
}

template <>
StringU16 ToStringU16(const StringU32 &input) {
  if (input.empty()) {
    return StringU16();
  }
  StringU16 output(
    simdutf::utf16_length_from_utf32(input.data(), input.size()), 0
  );
  if (!simdutf::convert_utf32_to_utf16(
    input.data(), input.size(), output.data()
  )) {
    throw ConvertError::FailedToConvert("Invalid UTF-32 string");
  }
  return output;
}

template <>
StringU32 ToStringU32(const StringU8 &input) {
  if (input.empty()) {
    return StringU32();
  }
  StringU32 output(
    simdutf::utf32_length_from_utf8(input.data(), input.size()), 0
  );
  if (!simdutf::convert_utf8_to_utf32(
    input.data(), input.size(), output.data()
  )) {
    throw ConvertError::FailedToConvert("Invalid UTF-8 string");
  }
  return output;
}

template <>
StringU32 ToStringU32(const StringU16 &input) {
  if (input.empty()) {
    return StringU32();
  }
  StringU32 output(
    simdutf::utf32_length_from_utf16(input.data(), input.size()), 0
  );
  if (!simdutf::convert_utf16_to_utf32(
    input.data(), input.size(), output.data()
  )) {
    throw ConvertError::FailedToConvert("Invalid UTF-16 string");
  }
  return output;
}

template <>
StringU32 ToStringU32(const StringU32 &input) {
  return input;
}

}

