///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include <aero-overlay/core/requirements.hpp>

#if !defined(_D2D1_H_)
    #include <d2d1.h>
    #include <d2d1helper.h>
    #include <dwrite.h>
#endif
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <aero-overlay/surface.hpp>

namespace aero::detail {
template<typename type>
void safe_release(
    type** data
)
{
    static_assert( std::is_base_of_v<IUnknown, type> );
    if( data && *data ) {
        ( *data )->Release();
        ( *data ) = nullptr;
    }
}

constexpr float round_float(
    const float value
)
{
    return static_cast<float>( static_cast<std::int32_t>( value ) );
}

}
