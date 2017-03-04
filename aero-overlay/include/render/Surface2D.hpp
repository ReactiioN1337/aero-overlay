#pragma once
#ifndef REACTIION_AERO_OVERLAY_SURFACE2D_HPP
#define REACTIION_AERO_OVERLAY_SURFACE2D_HPP

#include "Surface.hpp"
#include "Font2D.hpp"

namespace render {

class Surface2D :
    public Surface
{
public:
    Surface2D() = default;
    Surface2D( 
        const IDWriteFactory*       write_factory,
        const ID2D1SolidColorBrush* color_brush );
    ~Surface2D() override;
    bool initialize( const void* device ) override;
    bool begin_scene() override;
    void end_scene() override;
    void shutdown() override;
    void set_render_target( 
        const ID2D1HwndRenderTarget* render_target );
    void set_write_factory(
        const IDWriteFactory* write_factory );
    void set_color_brush( 
        const ID2D1SolidColorBrush* color_brush );

protected:
    void render_data() override;

private:
    ID2D1HwndRenderTarget* m_Direct2DHwndRenderTarget = nullptr;
    IDWriteFactory*        m_DirectWriteFactory = nullptr;
    ID2D1SolidColorBrush*  m_Diect2DColorBrush = nullptr;
};

}

#endif
