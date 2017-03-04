#pragma once
#ifndef REACTIION_AERO_OVERLAY_COLOR_HPP
#define REACTIION_AERO_OVERLAY_COLOR_HPP
#include "../stdafx.hpp"

namespace drawing {

class Color
{
    using Color_t = std::array<uint8_t, 4>;
public:
    ///-------------------------------------------------------------------------------------------------
    /// Default constructor.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///-------------------------------------------------------------------------------------------------
    Color() = default;

    ///-------------------------------------------------------------------------------------------------
    /// Constructor.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  r   The int32_t to process.
    /// @param  g   The int32_t to process.
    /// @param  b   The int32_t to process.
    /// @param  a   (Optional) The int32_t to process.
    ///-------------------------------------------------------------------------------------------------
    Color(
        const int32_t r,
        const int32_t g,
        const int32_t b,
        const int32_t a = 255 );

    ///-------------------------------------------------------------------------------------------------
    /// Constructor.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  hex_color   The hexadecimal color.
    ///-------------------------------------------------------------------------------------------------
    Color( const uint32_t hex_color );

    ///-------------------------------------------------------------------------------------------------
    /// Assignment operator.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  other   The other.
    ///
    /// @return A shallow copy of this object.
    ///-------------------------------------------------------------------------------------------------
    Color&                operator = ( const Color& other );

    ///-------------------------------------------------------------------------------------------------
    /// Array indexer operator.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  index   Zero-based index of the.
    ///
    /// @return The indexed value.
    ///-------------------------------------------------------------------------------------------------
    uint8_t&              operator [] ( const size_t index );

    ///-------------------------------------------------------------------------------------------------
    /// Array indexer operator.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  index   Zero-based index of the.
    ///
    /// @return The indexed value.
    ///-------------------------------------------------------------------------------------------------
    const uint8_t&        operator [] ( const size_t index ) const;

    ///-------------------------------------------------------------------------------------------------
    /// Function call operator.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @return The result of the operation.
    ///-------------------------------------------------------------------------------------------------
    uint8_t&              operator () ( const size_t index );

    ///-------------------------------------------------------------------------------------------------
    /// Function call operator.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @return The result of the operation.
    ///-------------------------------------------------------------------------------------------------
    const uint8_t&        operator () ( const size_t index ) const;

    ///-------------------------------------------------------------------------------------------------
    /// Ats the given index.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  index   Zero-based index of the.
    ///
    /// @return A reference to an uint8_t.
    ///-------------------------------------------------------------------------------------------------
    inline uint8_t&       at( const size_t index );

    ///-------------------------------------------------------------------------------------------------
    /// Ats the given index.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  index   Zero-based index of the.
    ///
    /// @return A reference to a const uint8_t.
    ///-------------------------------------------------------------------------------------------------
    inline const uint8_t& at( const size_t index ) const;

    ///-------------------------------------------------------------------------------------------------
    /// Converts this object to a hexadecimal.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @return This object as an uint32_t.
    ///-------------------------------------------------------------------------------------------------
    uint32_t              to_hex() const;

    ///-------------------------------------------------------------------------------------------------
    /// Sets the rgba channel.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  r   The int32_t to process.
    /// @param  g   The int32_t to process.
    /// @param  b   The int32_t to process.
    /// @param  a   (Optional) The int32_t to process.
    ///-------------------------------------------------------------------------------------------------
    void                  set(
        const int32_t r,
        const int32_t g,
        const int32_t b,
        const int32_t a = 255 );

    ///-------------------------------------------------------------------------------------------------
    /// Sets the given hexadecimal color.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  hex_color   The Hexadecimal color to set.
    ///-------------------------------------------------------------------------------------------------
    void                  set( const uint32_t hex_color );

    ///-------------------------------------------------------------------------------------------------
    /// Gets red channel.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @tparam T   Generic type parameter.
    ///
    /// @return A T.
    ///-------------------------------------------------------------------------------------------------
    template<typename T = int32_t>
    T                     r() const;

    ///-------------------------------------------------------------------------------------------------
    /// Gets green channel.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @tparam T   Generic type parameter.
    ///
    /// @return A T.
    ///-------------------------------------------------------------------------------------------------
    template<typename T = int32_t>
    T                     g() const;
    ///-------------------------------------------------------------------------------------------------
    /// Gets blue channel.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @tparam T   Generic type parameter.
    ///
    /// @return A T.
    ///-------------------------------------------------------------------------------------------------
    template<typename T = int32_t>
    T                     b() const;

    ///-------------------------------------------------------------------------------------------------
    /// Gets alpha channel.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @tparam T   Generic type parameter.
    ///
    /// @return A T.
    ///-------------------------------------------------------------------------------------------------
    template<typename T = int32_t>
    T                     a() const;

private:
    /// The color.
    Color_t m_Color;
};


inline uint8_t& Color::at(
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

inline const uint8_t& Color::at(
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

template<typename T>
T Color::r() const
{
    static_assert( std::is_arithmetic<T>::value, "Type T has to be arithmetic!" );
    return static_cast<T>( m_Color.at( 0 ) );
}

template<typename T>
T Color::g() const
{
    static_assert( std::is_arithmetic<T>::value, "Type T has to be arithmetic!" );
    return static_cast<T>( m_Color.at( 1 ) );
}

template<typename T>
T Color::b() const
{
    static_assert( std::is_arithmetic<T>::value, "Type T has to be arithmetic!" );
    return static_cast<T>( m_Color.at( 2 ) );
}

template<typename T>
T Color::a() const
{
    static_assert( std::is_arithmetic<T>::value, "Type T has to be arithmetic!" );
    return static_cast<T>( m_Color.at( 3 ) );
}

}

#endif
