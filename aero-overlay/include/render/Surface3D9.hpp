#pragma once
#ifndef REACTIION_AERO_OVERLAY_SURFACE3D9_HPP
#define REACTIION_AERO_OVERLAY_SURFACE3D9_HPP

#include "Surface.hpp"
/// DirectX 9 includes
#include <d3d9.h>
#include <d3dx9.h>

/// link the libraries for DirectX 9
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

namespace render {

class Surface3D9 :
    public Surface
{
public:
    Surface3D9() = default;
    Surface3D9( const ID3DXSprite* sprite );
    ~Surface3D9() override;
    bool initialize( const void* device ) override;
    bool begin_scene() override;
    void end_scene() override;
    void shutdown() override;
    void set_sprite( const ID3DXSprite* sprite );

protected:
    void render_data() override;

private:
    IDirect3DDevice9Ex* m_Direct3D9Device = nullptr;
    ID3DXSprite*        m_Direct3DXSprite = nullptr;
};

}

#endif
