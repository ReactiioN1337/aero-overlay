#pragma once
#ifndef REACTIION_AERO_OVERLAY_DEVICE2D_HPP
#define REACTIION_AERO_OVERLAY_DEVICE2D_HPP
#include "Overlay.hpp"
#include "Surface2D.hpp"

namespace render {

class Device2D :
    public Overlay
{
public:
    Device2D() = default;
    ~Device2D() override;
    bool create( const std::string& target_window_title ) override;
    bool render() override;
    void shutdown() override;

private:
    ID2D1Factory*              m_Direct2DFactory          = nullptr;
    ID2D1HwndRenderTarget*     m_Direct2DHwndRenderTarget = nullptr;
    IDWriteFactory*            m_DirectWriteFactory       = nullptr;
    ID2D1SolidColorBrush*      m_Diect2DColorBrush        = nullptr;
};

}

#endif
