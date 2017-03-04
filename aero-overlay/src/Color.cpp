#include <drawing/Color.hpp>
using namespace drawing;

Color::Color( 
    const int32_t r,
    const int32_t g,
    const int32_t b,
    const int32_t a )
{
    set( r, g, b, a );
}

Color::Color(
    const uint32_t hex_color )
{
    set( hex_color );
}

Color& Color::operator = (
    const Color& other )
{
    for( size_t i = 0; i < 4; ++i ) {
        at( 0 ) = other( i );
    }
    return *this;
}

uint8_t& Color::operator [] (
    const size_t index )
{
    return m_Color.at(
        std::clamp(
            index,
            static_cast<size_t>( 0 ),
            static_cast<size_t>( 1 )
        )
    );
}

const uint8_t& Color::operator [] (
    const size_t index ) const
{
    return m_Color.at(
        std::clamp(
            index,
            static_cast<size_t>( 0 ),
            static_cast<size_t>( 1 )
        )
    );
}

uint8_t& Color::operator () (
    const size_t index )
{
    return at( index );
}

const uint8_t& Color::operator () (
    const size_t index ) const
{
    return at( index );
}

uint32_t Color::to_hex() const
{
    return static_cast<uint32_t>(
        at( 3 ) << 24 | /// a
        at( 0 ) << 16 | /// r
        at( 1 ) <<  8 | /// g
        at( 2 )         /// b
    );
}

void Color::set(
    const int32_t r,
    const int32_t g,
    const int32_t b,
    const int32_t a )
{
    at( 0 ) = static_cast<uint8_t>( std::clamp( r, 0, 255 ) );
    at( 1 ) = static_cast<uint8_t>( std::clamp( g, 0, 255 ) );
    at( 2 ) = static_cast<uint8_t>( std::clamp( b, 0, 255 ) );
    at( 3 ) = static_cast<uint8_t>( std::clamp( a, 0, 255 ) );
}

void Color::set(
    const uint32_t hex_color )
{
    at( 3 ) = static_cast<uint8_t>( hex_color >> 24 & 0xFF );
    at( 0 ) = static_cast<uint8_t>( hex_color >> 16 & 0xFF );
    at( 1 ) = static_cast<uint8_t>( hex_color >>  8 & 0xFF );
    at( 2 ) = static_cast<uint8_t>( hex_color & 0xFF );
}
