///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include <aero-overlay/core/requirements.hpp>

namespace aero {
class color final
{
public:
    using type_rgba = std::array<std::uint8_t, 4>;

    constexpr static
    type_rgba hex_to_rgba(
        const std::uint32_t hex
    )
    {
        return type_rgba{
            static_cast<std::uint8_t>( hex >> 16 & 0xFF ),
            static_cast<std::uint8_t>( hex >> 8  & 0xFF ),
            static_cast<std::uint8_t>( hex       & 0xFF ),
            static_cast<std::uint8_t>( hex >> 24 & 0xFF )
        };
    }

    constexpr __forceinline static
    std::uint32_t rgba_to_hex(
        const type_rgba& rgba
    )
    {
        return rgba_to_hex( rgba[ 0 ], rgba[ 1 ], rgba[ 2 ], rgba[ 3 ] );
    }

    constexpr __forceinline static
    std::uint32_t rgba_to_hex(
        const std::uint8_t& r,
        const std::uint8_t& g,
        const std::uint8_t& b,
        const std::uint8_t& a = 255
    )
    {
        return static_cast<std::uint32_t>( a << 24 | r << 16 | g << 8 | b );
    }

    constexpr __forceinline static
    std::uint8_t r_channel(
        const std::uint32_t& hex_color
    )
    {
        return hex_color >> 16 & 0xFF;
    }

    constexpr __forceinline static
    std::uint8_t g_channel(
        const std::uint32_t& hex_color
    )
    {
        return hex_color >> 8 & 0xFF;
    }

    constexpr __forceinline static
    std::uint8_t b_channel(
        const std::uint32_t& hex_color
    )
    {
        return hex_color & 0xFF;
    }

    constexpr __forceinline static
    std::uint8_t a_channel(
        const std::uint32_t& hex_color
    )
    {
        return hex_color >> 24 & 0xFF;
    }


public:
    constexpr color() = default;

    ~color() = default;

    constexpr __forceinline
    color(
        const color& col
    ) = default;

    __forceinline
    color(
        color&& col
    ) noexcept
    {
        *this = std::move( col );
    }

    constexpr __forceinline
    color(
        const type_rgba& rgba
    ) : _rgba( rgba )
    { }

    constexpr __forceinline
    color(
        const std::uint32_t hex
    ) noexcept
    {
        set( hex );
    }

    template<typename type_r, typename type_g, typename type_b, typename type_a = std::int32_t>
    constexpr __forceinline
    color(
        const type_r r,
        const type_g g,
        const type_b b,
        const type_a a = static_cast<type_a>( 255 )
    ) noexcept
    {
        set( r, g, b, a );
    }

    constexpr __forceinline
    color& operator = (
        const color& rhs
    ) noexcept = default;

    __forceinline
    color& operator = (
        color&& rhs
    ) noexcept
    {
        _rgba = rhs._rgba;
        rhs.set( 0 );
        return *this;
    }

    __forceinline
    color& operator = (
        const type_rgba& rhs
    ) noexcept
    {
        _rgba = rhs;
        return *this;
    }

    constexpr __forceinline
    color& operator = (
        const std::uint32_t rhs
    ) noexcept
    {
        set( rhs );
        return *this;
    }

    constexpr __forceinline
    color& operator += (
        const type_rgba& rhs
    ) noexcept
    {
        for( std::size_t i = 0; i < 4; ++i ) {
            at( i ) = static_cast<std::uint8_t>( std::clamp( at( i ) + rhs[ i ], 0, 255 ) );
        }
        return *this;
    }

    constexpr __forceinline
    color& operator += (
        const color& rhs
    ) noexcept
    {
        *this += rhs._rgba;
        return *this;
    }

    constexpr __forceinline
    color& operator += (
        const std::uint32_t rhs
    ) noexcept
    {

        *this += hex_to_rgba( rhs );
        return *this;
    }

    constexpr __forceinline
    color& operator -= (
        const type_rgba& rhs
    ) noexcept
    {
        for( std::size_t i = 0; i < 4; ++i ) {
            at( i ) = static_cast<std::uint8_t>( std::clamp( at( i ) - rhs[ i ], 0, 255 ) );
        }
        return *this;
    }

    constexpr __forceinline
    color& operator -= (
        const color& rhs
    ) noexcept
    {
        *this -= rhs._rgba;
        return *this;
    }

    constexpr __forceinline
    color& operator -= (
        const std::uint32_t rhs
    ) noexcept
    {

        *this -= hex_to_rgba( rhs );
        return *this;
    }

    _NODISCARD __forceinline
    color operator + (
        const type_rgba& rhs
    ) const noexcept
    {
        auto lhs = *this;
        lhs += rhs;
        return lhs;
    }

    _NODISCARD __forceinline
    color operator + (
        const color& rhs
    ) const noexcept
    {
        auto lhs = *this;
        lhs += rhs;
        return lhs;
    }

    _NODISCARD __forceinline
    color operator + (
        const std::uint32_t rhs
    ) const noexcept
    {
        auto lhs = *this;
        lhs += rhs;
        return lhs;
    }

    _NODISCARD __forceinline
    color operator - (
        const type_rgba& rhs
    ) const noexcept
    {
        auto lhs = *this;
        lhs -= rhs;
        return lhs;
    }

    _NODISCARD __forceinline
    color operator - (
        const color& rhs
    ) const noexcept
    {
        auto lhs = *this;
        lhs -= rhs;
        return lhs;
    }

    _NODISCARD __forceinline
    color operator - (
        const std::uint32_t rhs
    ) const noexcept
    {
        auto lhs = *this;
        lhs -= rhs;
        return lhs;
    }

    _NODISCARD constexpr __forceinline
    bool operator == (
        const color& rhs
    ) const noexcept
    {
        return rgba_to_hex( _rgba ) == rhs.hex();
    }

    _NODISCARD constexpr __forceinline
    bool operator == (
        const std::uint32_t rhs
    ) const noexcept
    {
        return rgba_to_hex( _rgba ) == rhs;
    }

    _NODISCARD constexpr __forceinline
    bool operator != (
        const color& rhs
    ) const noexcept
    {
        return !( *this == rhs );
    }

    _NODISCARD constexpr __forceinline
    bool operator != (
        const std::uint32_t rhs
    ) const noexcept
    {
        return !( *this == rhs );
    }

    constexpr __forceinline
    std::uint8_t& at(
        const std::size_t index
    ) noexcept
    {
        return _rgba.at( std::clamp( index, static_cast<std::size_t>( 0 ), static_cast<std::size_t>( 3 ) ) );
    }

    _NODISCARD constexpr __forceinline
    const std::uint8_t& at(
        const std::size_t index
    ) const noexcept
    {
        return _rgba.at( std::clamp( index, static_cast<std::size_t>( 0 ), static_cast<std::size_t>( 3 ) ) );
    }

    template<typename type_r, typename type_g, typename type_b, typename type_a = std::int32_t>
    constexpr __forceinline
    void set(
        const type_r r,
        const type_g g,
        const type_b b,
        const type_a a = static_cast<type_a>( 255 )
    ) noexcept
    {
        at( 0 ) = static_cast<std::uint8_t>( std::clamp( static_cast<std::int32_t>( r ), 0, 255 ) );
        at( 1 ) = static_cast<std::uint8_t>( std::clamp( static_cast<std::int32_t>( g ), 0, 255 ) );
        at( 2 ) = static_cast<std::uint8_t>( std::clamp( static_cast<std::int32_t>( b ), 0, 255 ) );
        at( 3 ) = static_cast<std::uint8_t>( std::clamp( static_cast<std::int32_t>( a ), 0, 255 ) );
    }

    constexpr __forceinline
    void set(
        const std::uint32_t hex
    ) noexcept
    {
        _rgba = hex_to_rgba( hex );
    }

    constexpr __forceinline
    void set_opacity(
        const float value
    ) noexcept
    {
        at( 3 ) = static_cast<std::uint8_t>( std::clamp( value, 0.f, 1.f ) * 255.f );
    }

    constexpr __forceinline
    void set_norm(
        const float r,
        const float g,
        const float b,
        const float a = 1.f
    ) noexcept
    {
        set(
            std::clamp( r, 0.f, 1.f ) * 255.f,
            std::clamp( g, 0.f, 1.f ) * 255.f,
            std::clamp( b, 0.f, 1.f ) * 255.f,
            std::clamp( a, 0.f, 1.f ) * 255.f
        );
    }

    _NODISCARD constexpr __forceinline
    std::uint32_t hex(
    ) const noexcept
    {
        return rgba_to_hex( _rgba );
    }

    template<typename type = std::int32_t>
    _NODISCARD constexpr __forceinline
    type r(
    ) const noexcept
    {
        return static_cast<type>( at( 0 ) );
    }

    template<typename type = std::int32_t>
    _NODISCARD constexpr __forceinline
    type g(
    ) const noexcept
    {
        return static_cast<type>( at( 1 ) );
    }

    template<typename type = std::int32_t>
    _NODISCARD constexpr __forceinline
    type b(
    ) const noexcept
    {
        return static_cast<type>( at( 2 ) );
    }

    template<typename type = std::int32_t>
    _NODISCARD constexpr __forceinline
    type a(
    ) const noexcept
    {
        return static_cast<type>( at( 3 ) );
    }

    _NODISCARD constexpr __forceinline
    float r_norm(
    ) const noexcept
    {
        return r<float>() / 255.f;
    }

    _NODISCARD constexpr __forceinline
    float g_norm(
    ) const noexcept
    {
        return g<float>() / 255.f;
    }

    _NODISCARD constexpr __forceinline
    float b_norm(
    ) const noexcept
    {
        return b<float>() / 255.f;
    }

    _NODISCARD constexpr __forceinline
    float a_norm(
    ) const noexcept
    {
        return a<float>() / 255.f;
    }

private:
    type_rgba _rgba{};
};

}
