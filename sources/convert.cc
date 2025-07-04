
#include <cstdint>
#include <unicode/convert.h>
#include <unicode/length.h>

#ifdef UNICODE_USE_SIMDUTF
#include <simdutf.h>
#endif

namespace Unicode {

template <>
StringU8 ToStringU8(const StringU8 &input) {
  return input;
}
template <>
StringU16 ToStringU16(const StringU16 &input) {
  return input;
}
template <>
StringU32 ToStringU32(const StringU32 &input) {
  return input;
}
#ifdef UNICODE_USE_SIMDUTF

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
    throw ConvertError("Invalid UTF-16 string");
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
    throw ConvertError("Invalid UTF-32 string");
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
    throw ConvertError("Invalid UTF-8 string");
  }
  return output;
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
    throw ConvertError("Invalid UTF-32 string");
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
    throw ConvertError("Invalid UTF-8 string");
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
    throw ConvertError("Invalid UTF-16 string");
  }
  return output;
}
#else

template <typename Char>
inline auto CharToU(Char input) {
  static_assert(false, "Undefined character");
}
template <>
inline auto CharToU(Char8 input) {
  return static_cast<std::uint8_t>(input);
}
template <>
inline auto CharToU(Char16 input) {
  return static_cast<std::uint16_t>(input);
}
template <>
inline auto CharToU(Char32 input) {
  return static_cast<std::uint32_t>(input);
}
static StringU32 DecodeStringU8(const StringU8 &input) {
  StringU32 output(FindLength(input), 0);

  auto iterator = input.begin();

  for (auto &character : output) {
    if (CharToU(*iterator) < 0x80) {
      character = *iterator++;

    } else if ((CharToU(*iterator) & 0xE0) == 0xC0) {
      character  = (*iterator++ & 0x1F) << 6;
      character |= (*iterator++ & 0x3F);

    } else if ((CharToU(*iterator) & 0xF0) == 0xE0) {
      character  = (*iterator++ & 0x0F) << 12;
      character |= (*iterator++ & 0x3F) << 6;
      character |= (*iterator++ & 0x3F);

    } else if ((CharToU(*iterator) & 0xF8) == 0xF0) {
      character  = (*iterator++ & 0x07) << 18;
      character |= (*iterator++ & 0x3F) << 12;
      character |= (*iterator++ & 0x3F) << 6;
      character |= (*iterator++ & 0x3F);

    } else {
      throw ConvertError("Invalid UTF-8 string");
    }
  }
  return output;
}
static StringU32 DecodeStringU16(const StringU16 &input) {
  StringU32 output(FindLength(input), 0);

  auto iterator = input.begin();

  for (auto &character : output) {
    if (CharToU(*iterator) >= 0xD800 && CharToU(*iterator) <= 0xDBFF) {
      std::uint16_t high = CharToU(*iterator++);
      std::uint64_t low  = CharToU(*iterator++);

      if (low >= 0xDC00 && low <= 0xDFFF) {
        character = ((high - 0xD800) << 10) + (low - 0xDC00) + 0x10000;
      } else {
        throw ConvertError("Invalid UTF-16 string");
      }
    } else {
      character = CharToU(*iterator++);
    }
  }
  return output;
}
static std::uint64_t StringU8ByteCount(const StringU32 &input) {
  std::uint64_t output = 0;

  for (const auto &character : input) {
    if (CharToU(character) <= 0x7F) {
      output += 1;
    } else if (CharToU(character) <= 0x7FF) {
      output += 2;
    } else if (CharToU(character) <= 0xFFFF) {
      output += 3;
    } else if (CharToU(character) <= 0x10FFFF) {
      output += 4;
    } else {
      throw ConvertError("Invalid UTF-32 string");
    }
  }
  return output;
}
static StringU8 EncodeStringU8(const StringU32 &input) {
  StringU8 output(StringU8ByteCount(input), 0);

  auto iterator = output.begin();

  for (const auto &character : input) {
    if (CharToU(character) <= 0x7F) {
      *iterator++ = character;

    } else if (CharToU(character) <= 0x7FF) {
      *iterator++ = 0xC0 | (character >> 6);
      *iterator++ = 0x80 | (character & 0x3F);

    } else if (CharToU(character) <= 0xFFFF) {
      *iterator++ = 0xE0 | (character >> 12);
      *iterator++ = 0x80 | ((character >> 6) & 0x3F);
      *iterator++ = 0x80 | (character & 0x3F);

    } else if (CharToU(character) <= 0x10FFFF) {
      *iterator++ = 0xF0 | (character >> 18);
      *iterator++ = 0x80 | ((character >> 12) & 0x3F);
      *iterator++ = 0x80 | ((character >> 6) & 0x3F);
      *iterator++ = 0x80 | (character & 0x3F);

    } else {
      throw ConvertError("Invalid UTF-32 string");
    }
  }
  return output;
}
static std::uint64_t StringU16ByteCount(const StringU32 &input) {
  std::uint64_t output = 0;

  for (const auto &character : input) {
    if (CharToU(character) > 0xFFFF && CharToU(character) <= 0x10FFFF) {
      output++;

    } else if (CharToU(character) > 0x10FFFF) {
      throw ConvertError("Invalid UTF-32 string");
    }
    output++;
  }
  return output;
}
static StringU16 EncodeStringU16(const StringU32 &input) {
  StringU16 output(StringU16ByteCount(input), 0);

  auto iterator = output.begin();

  for (const auto &character : input) {
    if (CharToU(character) <= 0xFFFF) {
      *iterator++ = character;

    } else if (CharToU(character) <= 0x10FFFF) {
      *iterator++ = ((character - 0x10000) >> 10) + 0xD800;
      *iterator++ = ((character - 0x10000) & 0x3FF) + 0xDC00;

    } else {
      throw ConvertError("Invalid UTF-32 string");
    }
  }
  return output;
}
template <>
StringU8 ToStringU8(const StringU16 &input) {
  return EncodeStringU8(DecodeStringU16(input));
}
template <>
StringU8 ToStringU8(const StringU32 &input) {
  return EncodeStringU8(input);
}
template <>
StringU16 ToStringU16(const StringU8 &input) {
  return EncodeStringU16(DecodeStringU8(input));
}
template <>
StringU16 ToStringU16(const StringU32 &input) {
  return EncodeStringU16(input);
}
template <>
StringU32 ToStringU32(const StringU8 &input) {
  return DecodeStringU8(input);
}
template <>
StringU32 ToStringU32(const StringU16 &input) {
  return DecodeStringU16(input);
}
#endif // UNICODE_USE_SIMDUTF
}
