#include <render/Device3D9.hpp>
using namespace render;

Device3D9::~Device3D9()
{
    /// Call to a virtual function inside a destructor will be statically resolved
    [this]{
        shutdown();
    }();
}

bool Device3D9::create( const std::string& target_window_title )
{
    if( !Overlay::create( target_window_title ) ) {
        return false;
    }
    if( FAILED( Direct3DCreate9Ex( D3D_SDK_VERSION, &m_Direct3D9Ex ) ) ) {
        return false;
    }
    
    D3DPRESENT_PARAMETERS params;
    ZeroMemory( &params, sizeof( D3DPRESENT_PARAMETERS ) );

    params.Windowed               = TRUE;
    params.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    params.BackBufferFormat       = D3DFMT_A8R8G8B8;
    params.EnableAutoDepthStencil = TRUE;
    params.AutoDepthStencilFormat = D3DFMT_D16;
    params.MultiSampleType        = D3DMULTISAMPLE_NONE;
    params.PresentationInterval   = 0x80000000L;

    DWORD mulit_sample_quality = 0;
    if( SUCCEEDED( this->m_Direct3D9Ex->CheckDeviceMultiSampleType(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        D3DFMT_A8R8G8B8,
        true,
        D3DMULTISAMPLE_NONMASKABLE,
        &mulit_sample_quality ) ) ) {
        params.MultiSampleType    = D3DMULTISAMPLE_NONMASKABLE;
        params.MultiSampleQuality = mulit_sample_quality - 1;
    }
    if( FAILED( m_Direct3D9Ex->CreateDeviceEx(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        m_WndOverlay,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &params,
        nullptr,
        &m_Direct3D9Device ) ) ) {
        return false;
    }

    return true;
}

bool Device3D9::begin_scene()
{
    /// handle window messages
    if( !Overlay::begin_scene() ) {
        return false;
    }

    if( FAILED( m_Direct3D9Device->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET,
        0x00000000,
        1.f,
        0 )
    ) ) {
        return false;        
    }
    return in_foreground();
}

void Device3D9::end_scene()
{
    m_Direct3D9Device->EndScene();
    m_Direct3D9Device->PresentEx(
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0
    );
}

void Device3D9::shutdown()
{
    Overlay::shutdown();
    safe_release( &m_Direct3D9Ex );
    safe_release( &m_Direct3D9Device );
}
