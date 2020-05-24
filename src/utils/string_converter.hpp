///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include <aero-overlay/core/requirements.hpp>

namespace aero::detail {
template<typename type, typename from, typename fn>
std::basic_string<type> convert_string_func(
    const std::basic_string_view<from> input,
    const fn                           callback
)
{
    const auto len = input.length();
    std::basic_string<type> buf( len, 0 );
    callback( buf.data(), input.data(), len );
    return buf;
}
}

namespace aero::converter {
static std::string wstring_to_string(
    const std::wstring_view input
)
{
    return detail::convert_string_func<char>( input, std::wcstombs );
}

static std::wstring string_to_wstring(
    const std::string_view input
)
{
    return detail::convert_string_func<wchar_t>( input, std::mbstowcs );
}
}
