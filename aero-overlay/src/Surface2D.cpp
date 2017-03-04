#include <render/Surface2D.hpp>
using namespace drawing;
using namespace render;

Surface2D::~Surface2D()
{
    /// Call to a virtual function inside a destructor will be statically resolved
    DTOR_EXECUTE_VIRTUAL( shutdown );
}

Surface2D::Surface2D(
    const IDWriteFactory*       write_factory,
    const ID2D1SolidColorBrush* color_brush )
{
    set_write_factory( write_factory );
    set_color_brush( color_brush );
}

bool Surface2D::initialize( const void* device )
{
    if( device ) {
        m_Direct2DHwndRenderTarget = static_cast<ID2D1HwndRenderTarget*>(
            const_cast<void*>( device )
        );
    }
    m_Initialized = m_Diect2DColorBrush &&
        m_DirectWriteFactory &&
        m_Direct2DHwndRenderTarget;

    return m_Initialized;
}

bool Surface2D::begin_scene()
{
    if( m_Initialized ) {
        
    }
    return m_Initialized;
}

void Surface2D::end_scene()
{
    if( m_Initialized ) {
        render_data();
    }
}

void Surface2D::shutdown()
{
    
}

void Surface2D::set_render_target(
    const ID2D1HwndRenderTarget* render_target )
{
    m_Direct2DHwndRenderTarget = const_cast<ID2D1HwndRenderTarget*>( render_target );
}

void Surface2D::set_write_factory(
    const IDWriteFactory* write_factory )
{
    m_DirectWriteFactory = const_cast<IDWriteFactory*>( write_factory );
}

void Surface2D::set_color_brush(
    const ID2D1SolidColorBrush* color_brush )
{
    m_Diect2DColorBrush = const_cast<ID2D1SolidColorBrush*>( color_brush );
}

void Surface2D::render_data()
{
    if( !m_Initialized ) {
        return;
    }

    for( const auto& data : m_RectAngles ) {
        const auto& x   = static_cast<float>( std::get<0>( data ) );
        const auto& y   = static_cast<float>( std::get<1>( data ) );
        const auto& w   = static_cast<float>( std::get<2>( data ) );
        const auto& h   = static_cast<float>( std::get<3>( data ) );
        const auto clr  = std::get<4>( data ).to_hex();
        const auto rect = D2D1::RectF( x, y, x + w, y + h );
        m_Diect2DColorBrush->SetColor( D2D1::ColorF( clr ) );
        m_Direct2DHwndRenderTarget->FillRectangle( &rect, m_Diect2DColorBrush );
    }
    for( const auto& data : m_Lines ) {
        const auto& start_x = static_cast<float>( std::get<0>( data ) );
        const auto& start_y = static_cast<float>( std::get<1>( data ) );
        const auto& end_x   = static_cast<float>( std::get<2>( data ) );
        const auto& end_y   = static_cast<float>( std::get<3>( data ) );
        const auto clr      = std::get<4>( data ).to_hex();
        
        m_Diect2DColorBrush->SetColor( D2D1::ColorF( clr ) );
        /*m_Direct2DHwndRenderTarget->DrawLine(
            {
                start_x,
                start_y,
            },
            {
                end_x,
                end_y,
            },
            m_Diect2DColorBrush
        );*/
    }

    /// clear the old data
    m_Lines.clear();
    m_RectAngles.clear();
}
