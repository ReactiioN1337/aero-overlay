///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#include "d2d_surface.hpp"
#include "d2d_font.hpp"
#include "../utils/string_converter.hpp"

using namespace aero;

api_status d2d_surface::initialize(
    void* overlay_handle,
    void* target_handle
)
{
    auto* const hwnd = static_cast<HWND>( overlay_handle );
    if( !hwnd ) {
        return api_status::missing_window_handle;
    }

    release();

    auto status = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_MULTI_THREADED,
        __uuidof( ID2D1Factory ),
        nullptr,
        reinterpret_cast<void**>( &_factory )
    );

    if( FAILED( status ) ) {
        return api_status::failed_to_create_surface_factory;
    }

    status = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof( IDWriteFactory ),
        reinterpret_cast<IUnknown**>( &_write_factory )
    );

    if( FAILED( status ) ) {
        return api_status::failed_to_create_surface_factory;
    }

    RECT rect{};
    GetClientRect( hwnd, &rect );

    _width  = static_cast<float>( rect.right - rect.left );
    _height = static_cast<float>( rect.bottom - rect.top );
    status  = _factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_HARDWARE,
            D2D1::PixelFormat(
                DXGI_FORMAT_UNKNOWN,
                D2D1_ALPHA_MODE_PREMULTIPLIED // todo: check
            )
        ),
        D2D1::HwndRenderTargetProperties(
            hwnd,
            D2D1::SizeU(
                static_cast<std::uint32_t>( _width ),
                static_cast<std::uint32_t>( _height )
            ),
            D2D1_PRESENT_OPTIONS_IMMEDIATELY
        ),
        &_render_target
    );

    if( FAILED( status ) ) {
        return api_status::failed_to_create_render_target;
    }

    _render_target->SetAntialiasMode( D2D1_ANTIALIAS_MODE_ALIASED );

    status = _render_target->CreateSolidColorBrush(
        D2D1::ColorF( 0xFFFFFFFF ),
        &_brush
    );

    _window_handle = target_handle;

    return SUCCEEDED( status )
        ? api_status::success
        : api_status::failed_to_create_render_target;
}

void d2d_surface::release()
{
    detail::safe_release( &_brush );
    detail::safe_release( &_render_target );
    detail::safe_release( &_write_factory );
    detail::safe_release( &_factory );

    for( const auto& kp : _fonts ) {
        kp.second->release();
    }

    _fonts.clear();
}

font_ptr d2d_surface::add_font(
    const std::string&  name,
    const std::string&  family,
    const float         height,
    const std::uint32_t weight,
    const std::uint32_t flags
)
{
    if( const auto font = get_font( name ) ) {
        return font;
    }

    auto font = std::make_shared<d2d_font>( _write_factory, family, height, weight, flags );
    if( font->create() != api_status::success ) {
        return nullptr;
    }

    _fonts.insert( std::make_pair( name, std::move( font ) ) );

    return get_font( name );
}

void d2d_surface::border_box(
    const float  x,
    const float  y,
    const float  w,
    const float  h,
    float        thickness,
    const color& col,
    float        thickness_outside,
    float        thickness_inside,
    const color& col_outside,
    const color& col_inside
)
{
    thickness = detail::round_float( thickness );

    // top
    rect( x, y, w, thickness, col, 0.f );
    // bottom
    rect( x, y + h - thickness, w, thickness, col, 0.f );
    // left
    rect( x, y, thickness, h, col, 0.f );
    // right
    rect( x + w - thickness, y, thickness, h, col, 0.f );

    if( thickness_outside >= 1.f ) {
        thickness_outside = detail::round_float( thickness_outside );
        const auto mod    = thickness_outside * 2.f;

        border_box(
            x - thickness_outside,
            y - thickness_outside,
            w + mod,
            h + mod,
            thickness_outside,
            col_outside,
            0.f,
            0.f
        );
    }
    if( thickness_inside >= 1.f ) {
        thickness_inside = detail::round_float( thickness_inside );
        const auto mod   = thickness * 2.f;

        border_box(
            x + thickness,
            y + thickness,
            w - mod,
            h - mod,
            thickness_inside,
            col_inside,
            0.f,
            0.f
        );
    }
}

void d2d_surface::line(
    const float  start_x,
    const float  start_y,
    const float  end_x,
    const float  end_y,
    const color& col
)
{
    if( _render_target ) {
        set_color( col );

        _render_target->DrawLine(
            D2D1::Point2F( start_x, start_y ),
            D2D1::Point2F( end_x, end_y ),
            _brush
        );
    }
}

void d2d_surface::rect(
    const float  x,
    const float  y,
    const float  w,
    const float  h,
    const color& col,
    const float  thickness_outside,
    const color& col_outside
)
{
    if( thickness_outside >= 1.f ) {
        const auto value = detail::round_float( thickness_outside );
        const auto mod   = value * 2.f;

        rect(
            x - value,
            y - value,
            w + mod,
            h + mod,
            col_outside,
            0.f
        );
    }

    set_color( col );
    _render_target->FillRectangle(
        D2D1::RectF( x, y, x + w, y + h ),
        _brush
    );
}

void d2d_surface::text_ansii(
    const float            x,
    const float            y,
    const font_ptr&        font,
    const color&           col,
    const std::string_view text
)
{
    if( !text.empty() ) {
        text_unicode( x, y, font, col, converter::string_to_wstring( text ) );
    }
}

void d2d_surface::text_unicode(
    const float             x,
    const float             y,
    const font_ptr&         font,
    const color&            col,
    const std::wstring_view text
)
{
    if( font && !text.empty() ) {
        D2D1_RECT_F font_rect{ x, y, 0.f, 0.f };
        font->get_text_size( text, &font_rect.right, &font_rect.bottom );

        font_rect.right  += x + 1.f;
        font_rect.bottom += y + 1.f;

        set_color( col );

        _render_target->DrawText(
            text.data(),
            static_cast<std::uint32_t>( text.length() ),
            font->get_data<IDWriteTextFormat>(),
            &font_rect,
            _brush
        );
    }
}

bool d2d_surface::begin_scene()
{
    if( !_render_target ) {
        return false;
    }

    _render_target->BeginDraw();
    _render_target->SetTransform( D2D1::Matrix3x2F::Identity() );
    _render_target->Clear();

    _visible = _window_handle == GetForegroundWindow();

    if( _visible && !_callbacks.empty()  ) {
        for( const auto& fn : _callbacks ) {
            fn();
        }
    }

    return true;
}

void d2d_surface::end_scene()
{
    if( _render_target ) {
        _render_target->EndDraw();
    }
}

void d2d_surface::set_color(
    const color& col
)
{
    const auto hex = col.hex();
    if( hex != _last_color ) {
        _brush->SetColor( D2D1::ColorF( hex ) );
        _last_color = hex;
    }
}
