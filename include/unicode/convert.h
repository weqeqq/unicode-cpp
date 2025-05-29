
#pragma once 

#include <unicode/type.h>
#include <unicode/error.h>

namespace Unicode {

EXCEPTION_DEFINE_NAMESPACE(
  ConvertError, 
  EXCEPTION_ROOT,
  EXCEPTION_MESSAGE(FailedToConvert);
);
EXCEPTION_DEFINE_MESSAGE(
  FailedToConvert, 
  ConvertError
);
template <typename Char>
StringU8 ToStringU8(const BasicString<Char> &input) {
  static_assert(false, "Undefined character type");

  return StringU8();
}
template <typename Char>
StringU8 ToStringU8(const Char *input) {
  return ToStringU8(BasicString<Char>(input));
}
template <>
StringU8 ToStringU8(const StringU8 &input);

template <>
StringU8 ToStringU8(const StringU16 &input);

template <>
StringU8 ToStringU8(const StringU32 &input);

template <typename Char>
StringU16 ToStringU16(const BasicString<Char> &input) {
  static_assert(false, "Undefined character type");

  return StringU16();
}
template <typename Char>
StringU16 ToStringU16(const Char *input) {
  return ToStringU16(BasicString<Char>(input));
}
template <>
StringU16 ToStringU16(const StringU8 &input);

template <>
StringU16 ToStringU16(const StringU16 &input);

template <>
StringU16 ToStringU16(const StringU32 &input);

template <typename Char>
StringU32 ToStringU32(const BasicString<Char> &input) {
  static_assert(false, "Undefined character type");

  return StringU32();
}
template <typename Char>
StringU32 ToStringU32(const Char *input) {
  return ToStringU32(BasicString<Char>(input));
}
template <>
StringU32 ToStringU32(const StringU8 &input);

template <>
StringU32 ToStringU32(const StringU16 &input);

template <>
StringU32 ToStringU32(const StringU32 &input);

namespace Detail {

template <typename Char1, typename Char2>
static constexpr bool CharEqual = std::is_same_v<Char1, Char2>;

template <typename Char>
static constexpr bool IsChar8 = CharEqual<Char, Char8>;

template <typename Char>
static constexpr bool IsChar16 = CharEqual<Char, Char16>;

template <typename Char>
static constexpr bool IsChar32 = CharEqual<Char, Char32>;

};
template <typename OutputChar, typename Char>
BasicString<OutputChar> Convert(const BasicString<Char> &input) {
  if constexpr (Detail::IsChar8<OutputChar>) {
    return ToStringU8(input);

  } else if constexpr (Detail::IsChar16<OutputChar>) {
    return ToStringU16(input);

  } else if constexpr (Detail::IsChar32<OutputChar>) {
    return ToStringU32(input);

  } else {
    static_assert(false, "Undefined character type");
  }
}
}
