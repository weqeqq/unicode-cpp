
#pragma once 

#include <string>

namespace Unicode {

using Char8  = char;
using Char16 = char16_t;
using Char32 = char32_t;

template <typename Char>
using BasicString = std::basic_string<Char>;

using StringU8  = BasicString<Char8>;
using StringU16 = BasicString<Char16>;
using StringU32 = BasicString<Char32>;

}

