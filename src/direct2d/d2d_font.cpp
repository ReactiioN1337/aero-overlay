///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#include "d2d_font.hpp"
#include "../utils/string_converter.hpp"

using namespace aero;

d2d_font::d2d_font(
    IDWriteFactory*     factory,
    const std::string&  family,
    const float         height,
    const std::uint32_t weight,
    const std::uint32_t flags
)
    : font( family, height, weight, flags )
    , _factory( factory )
{ }

d2d_font::~d2d_font()
{
    // virtual functions can not be executed inside a destructor
    kill();
}

void d2d_font::kill()
{
    release();
}

api_status d2d_font::create()
{
    if( !_factory ) {
        return api_status::missing_factory_to_create_font;
    }
    if( _family.empty() ) {
        return api_status::missing_family_to_create_font;
    }

    const auto family = converter::string_to_wstring( _family );
    const auto status = _factory->CreateTextFormat(
        family.data(),
        nullptr,
        static_cast<DWRITE_FONT_WEIGHT>( _weight ),
        DWRITE_FONT_STYLE_NORMAL,
        static_cast<DWRITE_FONT_STRETCH>( _flags == 0 ? DWRITE_FONT_STRETCH_NORMAL : _flags ),
        _height,
        L"en-Us",
        &_data
    );

    return SUCCEEDED( status )
        ? api_status::success
        : api_status::failed_to_create_font;
}

void d2d_font::release()
{
    detail::safe_release( &_data );
}

void** d2d_font::address_of() noexcept
{
    return reinterpret_cast<void**>( &_data );
}

void* d2d_font::get_data() const noexcept
{
    return _data;
}

void d2d_font::get_text_size(
    const std::wstring_view text,
    float* const            width,
    float* const            height
)
{
    if( !text.empty() ) {
        if( !width && !height ) {
            return;
        }

        IDWriteTextLayout* layout = nullptr;
        const auto status = _factory->CreateTextLayout(
            text.data(),
            static_cast<std::uint32_t>( text.length() ),
            _data,
            4096.f,
            4096.f,
            &layout
        );

        if( SUCCEEDED( status ) ) {
            DWRITE_TEXT_METRICS metrics{};
            if( SUCCEEDED( layout->GetMetrics( &metrics ) ) ) {
                if( width ) {
                    *width = metrics.width;
                }
                if( height ) {
                    *height = metrics.height;
                }
            }
            detail::safe_release( &layout );
        }
    }
}

void d2d_font::get_text_size(
    const std::string_view text,
    float* const           width,
    float* const           height
)
{
    if( !text.empty() ) {
        get_text_size( converter::string_to_wstring( text ), width, height );
    }
}
