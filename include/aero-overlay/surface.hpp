///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include <aero-overlay/core/color.hpp>
#include <aero-overlay/font.hpp>

namespace aero {
class surface abstract
{
protected:
    using font_map           = std::unordered_map<std::string, font_ptr>;
    using render_callback_fn = std::function<void()>;
    using render_callbacks   = std::vector<render_callback_fn>;

public:
    virtual ~surface() = default;

    virtual api_status initialize(
        void* overlay_handle,
        void* target_handle
    ) = 0;

    virtual void release() = 0;

    virtual font_ptr add_font(
        const std::string& name,
        const std::string& family,
        float              height,
        std::uint32_t      weight = 400,
        std::uint32_t      flags  = 0
    ) = 0;

    virtual void border_box(
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
    ) = 0;

    virtual void line(
        float        start_x,
        float        start_y,
        float        end_x,
        float        end_y,
        const color& col
    ) = 0;

    virtual void rect(
        float        x,
        float        y,
        float        w,
        float        h,
        const color& col,
        float        thickness_outside = 0.f,
        const color& col_outside = 0xFF010101
    ) = 0;

    virtual void text_ansii(
        float            x,
        float            y,
        const font_ptr&  font,
        const color&     col,
        std::string_view text
    ) = 0;

    virtual void text_unicode(
        float             x,
        float             y,
        const font_ptr&   font,
        const color&      col,
        std::wstring_view text
    ) = 0;

    virtual bool begin_scene() = 0;

    virtual void end_scene() = 0;

    _NODISCARD constexpr
    float get_width() const noexcept
    {
        return _width;
    }

    _NODISCARD constexpr
    float get_height() const noexcept
    {
        return _height;
    }

    _NODISCARD constexpr
    bool is_visible() const noexcept
    {
        return _visible;
    }

    void add_callback(
        render_callback_fn callback
    )
    {
        if( callback ) {
            _callbacks.emplace_back( std::move( callback ) );
        }
    }

    _NODISCARD
    font_ptr get_font(
        const std::string& name
    ) const noexcept
    {
        return _fonts.count( name )
            ? _fonts.at( name )
            : nullptr;
    }

    template<typename type>
    void get_text_size(
        const std::basic_string_view<type> text,
        const font_ptr&                    font,
        float* const                       width,
        float* const                       height
    ) const noexcept
    {
        static_assert( std::is_same_v<char, type> || std::is_same_v<wchar_t, type> );

        if( font ) {
            font->get_text_size( text, width, height );
        }
    }

    template<typename type>
    void get_text_size(
        const std::basic_string_view<type> text,
        const std::string&                 font_name,
        float* const                       width,
        float* const                       height
    ) const noexcept
    {
        get_text_size( text, get_font( font_name ), width, height );
    }

    template<std::size_t buffer_size = 1024, typename ...arguments>
    void text(
        const float x,
        const float y,
        const font_ptr& font,
        const color& col,
        const char* fmt,
        arguments&& ...args
    )
    {
        if( fmt && font ) {
            const std::size_t len = std::snprintf( nullptr, 0, fmt, args... ) + 1;
            const auto final_len  = std::min( len, buffer_size + 1 );

            char buffer[ buffer_size + 1 ]{};
            std::snprintf( buffer, final_len, fmt, args... );

            text_ansii( x, y, font, col, std::string( buffer, buffer + final_len - 1 ) );
        }
    }

    template<std::size_t buffer_size = 1024, typename ...arguments>
    void text(
        const float     x,
        const float     y,
        const font_ptr& font,
        const color&    col,
        const wchar_t*  fmt,
        arguments&&     ...args
    )
    {
        if( fmt && font ) {
            const std::size_t len = std::swprintf( nullptr, 0, fmt, args... ) + 1;
            const auto final_len  = std::min( len, buffer_size + 1 );

            wchar_t buffer[ buffer_size + 1 ]{};
            std::swprintf( buffer, final_len, fmt, args... );

            text_unicode( x, y, font, col, std::wstring( buffer, buffer + final_len - 1 ) );
        }
    }

protected:
    font_map         _fonts{};
    render_callbacks _callbacks{};
    float            _width   = 0.f;
    float            _height  = 0.f;
    bool             _visible = false;
};

using surface_ptr = std::shared_ptr<surface>;
}
