///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include <aero-overlay/core/requirements.hpp>

namespace aero {
enum class api_status
    : std::uint32_t
{
    success = 0,
    missing_factory_to_create_font,
    missing_family_to_create_font,
    failed_to_create_font,
    missing_window_handle,
    missing_window_title,
    failed_to_create_surface_factory,
    failed_to_create_render_target,
    missing_aero_feature,
    failed_to_register_window,
    failed_to_create_window,
    failed_to_make_window_transparent
};

constexpr std::uint32_t api_status_code(
    const api_status status
)
{
    return static_cast<std::underlying_type_t<api_status>>( status );
}
}
