#pragma once
#ifndef REACTIION_AERO_OVERLAY_SURFACE_HPP
#define REACTIION_AERO_OVERLAY_SURFACE_HPP

#include "../drawing/Color.hpp"

namespace render {

class Surface
{
public:
    static constexpr size_t MAX_RENDER_ELEMENTS = 512;

private:
    using RenderObj  = std::tuple<
        int32_t,       /// x position
        int32_t,       /// y position
        int32_t,       /// width
        int32_t,       /// height
        drawing::Color /// obv. the color
    >;
    using RenderData = std::vector<RenderObj>;

public:
    Surface() = default;
    virtual ~Surface();
    virtual bool initialize( const void* device ) = 0;
    virtual bool begin_scene() = 0;
protected:
    virtual void render_data() = 0;
public:
    virtual void end_scene() = 0;
    virtual void shutdown() = 0;
    virtual void border_box(
        const int32_t x,
        const int32_t y,
        const int32_t w,
        const int32_t h,
        const int32_t thickness,
        const drawing::Color& color );

    virtual void border_box_outlined(
        const int32_t x,
        const int32_t y,
        const int32_t w,
        const int32_t h,
        const int32_t thickness,
        const drawing::Color& color,
        const drawing::Color& outlined  = 0xFF000000,
        const int32_t thickness_outline = 1 );

    virtual void rect(
        const int32_t         x,
        const int32_t         y,
        const int32_t         w,
        const int32_t         h,
        const drawing::Color& color );

    virtual void rect_outlined(
        const int32_t x,
        const int32_t y,
        const int32_t w,
        const int32_t h,
        const int32_t thickness,
        const drawing::Color& color,
        const drawing::Color& outlined = 0xFF000000 );

    virtual void line(
        const int32_t         start_x,
        const int32_t         start_y,
        const int32_t         end_x,
        const int32_t         end_y,
        const drawing::Color& color );

protected:
    template<typename T1, typename T2, typename T3, typename T4>
    void insert_line(
        const T1              start_x,
        const T2              start_y,
        const T3              end_x,
        const T4              end_y,
        const drawing::Color& color );

    template<typename T1, typename T2, typename T3, typename T4>
    void insert_rect_angle(
        const T1              x,
        const T2              y,
        const T3              w,
        const T4              h,
        const drawing::Color& color );

protected:
    bool       m_Initialized = false;
    RenderData m_Lines;
    RenderData m_RectAngles;
};

template<typename T1, typename T2, typename T3, typename T4>
void Surface::insert_line(
    const T1              start_x,
    const T2              start_y,
    const T3              end_x,
    const T4              end_y,
    const drawing::Color& color )
{
    static_assert( std::is_arithmetic<T1>::value, "Type T1 has to be arithmetic" );
    static_assert( std::is_arithmetic<T2>::value, "Type T2 has to be arithmetic" );
    static_assert( std::is_arithmetic<T3>::value, "Type T3 has to be arithmetic" );
    static_assert( std::is_arithmetic<T4>::value, "Type T4 has to be arithmetic" );

    if( m_Lines.size() >= MAX_RENDER_ELEMENTS - 1 ) {
        render_data();
    }

    m_Lines.push_back( 
        std::make_tuple(
            static_cast<int32_t>( start_x ),
            static_cast<int32_t>( start_y ),
            static_cast<int32_t>( end_x ),
            static_cast<int32_t>( end_y ),
            color
        )
    );
}

template<typename T1, typename T2, typename T3, typename T4>
void Surface::insert_rect_angle(
    const T1              x,
    const T2              y,
    const T3              w,
    const T4              h,
    const drawing::Color& color )
{
    static_assert( std::is_arithmetic<T1>::value, "Type T1 has to be arithmetic" );
    static_assert( std::is_arithmetic<T2>::value, "Type T2 has to be arithmetic" );
    static_assert( std::is_arithmetic<T3>::value, "Type T3 has to be arithmetic" );
    static_assert( std::is_arithmetic<T4>::value, "Type T4 has to be arithmetic" );

    if( m_RectAngles.size() >= MAX_RENDER_ELEMENTS - 1 ) {
        render_data();
    }

    m_RectAngles.push_back( 
        std::make_tuple(
            static_cast<int32_t>( x ),
            static_cast<int32_t>( y ),
            static_cast<int32_t>( w ),
            static_cast<int32_t>( h ),
            color
        )
    );
}

}

#endif
