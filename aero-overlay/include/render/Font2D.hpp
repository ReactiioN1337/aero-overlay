#pragma once
#ifndef REACTIION_AERO_OVERLAY_FONT2D_HPP
#define REACTIION_AERO_OVERLAY_FONT2D_HPP

/// the Font interface
#include "Font.hpp"

/// disable the compiler warning C4005
/// mixing DirectX with Direct2D will result in a ton of
/// compiler warnings (previous definition XXX)
#pragma warning( disable : 4005 )
/// Direct2D + Direct Draw include files
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

/// link the libraries for Direct2D + Direct Draw
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )

/// enable the compiler warning C4005
#pragma warning( default : 4005 )

namespace render {

class Font2D :
    public Font
{
public:
    Font2D() = default;
    Font2D( 
        const IDWriteFactory* write_factory );
    ~Font2D() override;
    void               shutdown() override;
    bool               create() override;
    IDWriteTextFormat* get_font() const;
    void               set_device( 
        const IDWriteFactory* write_factory );

private:
    IDWriteFactory*    m_Direct2DWFactory = nullptr;
    IDWriteTextFormat* m_Direct2DFont     = nullptr;
};

}

#endif
