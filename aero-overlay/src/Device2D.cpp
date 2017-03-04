#include <render/Device2D.hpp>
using namespace render;

Device2D::~Device2D()
{
    /// Call to a virtual function inside a destructor will be statically resolved
    [this]{
        shutdown();
    }();
}

bool Device2D::create( const std::string& target_window_title )
{
    if( !Overlay::create( target_window_title ) ) {
        return false;
    }
    if( FAILED( D2D1CreateFactory( D2D1_FACTORY_TYPE_MULTI_THREADED,
                                   __uuidof( ID2D1Factory ),
                                   nullptr,
                                   reinterpret_cast<void**>( &m_Direct2DFactory ) 
    ) ) ) {
        return false;
    }

    RECT rect;
    GetClientRect( m_WndOverlay, &rect );

    auto size = D2D1::SizeU( rect.right - rect.left, rect.bottom - rect.top );
    if( FAILED( 
        m_Direct2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(
                D2D1_RENDER_TARGET_TYPE_HARDWARE,
                D2D1::PixelFormat(
                    DXGI_FORMAT_UNKNOWN,
                    D2D1_ALPHA_MODE_PREMULTIPLIED 
                )
            ),
            D2D1::HwndRenderTargetProperties(
                m_WndOverlay,
                size,
                D2D1_PRESENT_OPTIONS_IMMEDIATELY
            ),
            &m_Direct2DHwndRenderTarget ) ) ) {
        return false;
    }

    if( FAILED( DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED,
                                     __uuidof( IDWriteFactory ),
                                     reinterpret_cast<IUnknown**>( &m_DirectWriteFactory ) )
    ) ) {
        return false;
    }
    if( FAILED( m_Direct2DHwndRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF( 0xFFFFFFFF ),
        &m_Diect2DColorBrush ) 
    ) ) {
        return false;
    }

    return true;
}

bool Device2D::begin_scene()
{
    /// handle window messages
    if( !Overlay::begin_scene() ) {
        return false;
    }

    m_Direct2DHwndRenderTarget->BeginDraw();
    m_Direct2DHwndRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );
    m_Direct2DHwndRenderTarget->Clear();

    return in_foreground();
}

void Device2D::end_scene()
{
    m_Direct2DHwndRenderTarget->EndDraw();
}

void Device2D::shutdown()
{
    Overlay::shutdown();
    safe_release( &m_Direct2DFactory );
    safe_release( &m_Direct2DHwndRenderTarget );
    safe_release( &m_DirectWriteFactory );
    safe_release( &m_Diect2DColorBrush );
}
