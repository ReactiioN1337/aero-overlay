///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include "pre_include.hpp"

namespace aero {
class d2d_surface final
    : public surface
{
public:
    api_status initialize(
        void* overlay_handle,
        void* target_handle
    ) override;

    void release() override;

    font_ptr add_font(
        const std::string& name,
        const std::string& family,
        float              height,
        std::uint32_t      weight,
        std::uint32_t      flags
    ) override;

    void border_box(
        float        x,
        float        y,
        float        w,
        float        h,
        float        thickness,
        const color& col,
        float        thickness_outside = 0.f,
        float        thickness_inside  = 0.f,
        const color& col_outside       = 0xFF010101,
        const color& col_inside        = 0xFF010101
    ) override;

    void line(
        float        start_x,
        float        start_y,
        float        end_x,
        float        end_y,
        const color& col
    ) override;

    void rect(
        float        x,
        float        y,
        float        w,
        float        h,
        const color& col,
        float        thickness_outside = 0.f,
        const color& col_outside = 0xFF010101
    ) override;

    void text_ansii(
        float            x,
        float            y,
        const font_ptr&  font,
        const color&     col,
        std::string_view text
    ) override;

    void text_unicode(
        float             x,
        float             y,
        const font_ptr&   font,
        const color&      col,
        std::wstring_view text
    ) override;

    bool begin_scene() override;

    void end_scene() override;

    void set_color(
        const color& col
    );

private:
    ID2D1Factory*          _factory       = nullptr;
    IDWriteFactory*        _write_factory = nullptr;
    ID2D1HwndRenderTarget* _render_target = nullptr;
    ID2D1SolidColorBrush*  _brush         = nullptr;
    void*                  _window_handle = nullptr;
    std::uint32_t          _last_color    = 0;
};
}
