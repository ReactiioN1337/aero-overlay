#pragma once
#ifndef REACTIION_AERO_OVERLAY_FONT3D9_HPP
#define REACTIION_AERO_OVERLAY_FONT3D9_HPP

/// the Font interface
#include "Font.hpp"

/// DirectX 9 includes
#include <d3d9.h>
#include <d3dx9.h>

/// link the libraries for DirectX 9
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

namespace render {

class Font3D9 :
    public Font
{
public:
    Font3D9() = default;
    Font3D9( const IDirect3DDevice9Ex* device );
    ~Font3D9() override;
    void       shutdown() override;
    bool       create() override;
    ID3DXFont* get_font() const;
    void       set_device( const IDirect3DDevice9Ex* device );    

protected:
    IDirect3DDevice9Ex* m_Direct3D9Device = nullptr;
    ID3DXFont*          m_Direct3DXFont   = nullptr;
};

}

#endif
