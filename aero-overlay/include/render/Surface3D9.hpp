#pragma once
#ifndef REACTIION_AERO_OVERLAY_SURFACE3D9_HPP
#define REACTIION_AERO_OVERLAY_SURFACE3D9_HPP

#include "Surface.hpp"
#include "Font3D9.hpp"

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
    Font_t add_font(
        const std::string& name,
        const std::string& definition,
        const int32_t      height,
        const int32_t      weight,
        const int32_t      flags ) override;
    void end_scene() override;
    void shutdown() override;
    void text(
        const int32_t         x,
        const int32_t         y,
        const Font_t&         font,
        const drawing::Color& color,
        const std::string&    message ) override;
    void set_sprite( const ID3DXSprite* sprite );

protected:
    void render_data() override;

private:
    IDirect3DDevice9Ex* m_Direct3D9Device = nullptr;
    ID3DXSprite*        m_Direct3DXSprite = nullptr;
};

}

#endif
