#include <render/Overlay.hpp>
#include <render/Device2D.hpp>
#include <render/Device3D9.hpp>
using namespace render;

Overlay::Overlay()
{
    m_ClassName.assign( generate_random_string( 128 ) );
    m_WindowTitle.assign( generate_random_string( 128 ) );
}

Overlay::~Overlay()
{
}

bool Overlay::create(
    const std::string& target_window_title )
{
    if( target_window_title.empty() ) {
        return false;
    }

    m_WndTarget = FindWindowA( nullptr, target_window_title.c_str() );
    if( !m_WndTarget ) {
        return false;
    }

#if defined(_UNICODE)
    const auto class_name   = convert_string_to_wide( m_ClassName );
    const auto window_title = convert_string_to_wide( m_WindowTitle );
#endif

    WNDCLASSEX window_class_ex =
    {
        sizeof( WNDCLASSEX ),
        0,
        window_procedure,
        0,
        0,
        nullptr,
        LoadIcon( nullptr, IDI_APPLICATION ),
        LoadCursor( nullptr, IDC_ARROW ),
        nullptr,
        nullptr,
    #if defined(_UNICODE)
        class_name.c_str(),
    #else
        m_ClassName.c_str(),
    #endif 
        LoadIcon( nullptr, IDI_APPLICATION )
    };

    if( !RegisterClassEx( &window_class_ex ) ) {
        return false;
    }

    m_WndOverlay = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
    #ifdef _UNICODE
        class_name.c_str(),
        window_title.c_str(),
    #else
        m_ClassName.c_str(),
        m_WindowTitle.c_str(),
    #endif
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        nullptr, 
        nullptr, 
        nullptr, 
        nullptr 
    );

    if( !m_WndOverlay ) {
        return false;
    }
    if( !SetLayeredWindowAttributes( 
        m_WndOverlay,
        RGB( NULL, NULL, NULL ),
        255,
        ULW_COLORKEY | LWA_ALPHA ) ) {
        return false;
    }

    auto window_props = get_window_props( m_WndTarget );    
    if( FAILED( DwmExtendFrameIntoClientArea(
        m_WndOverlay,
        reinterpret_cast<const MARGINS*>( &window_props ) )
    ) ) {
        return false;
    }

    ShowWindow( m_WndOverlay, SW_SHOWDEFAULT );
    if( !UpdateWindow( m_WndOverlay ) ) {
        return false;
    }

    scale_overlay();
    return true;
}

bool Overlay::in_foreground() const
{
    return m_WndTarget && GetForegroundWindow() == m_WndTarget;
}

bool Overlay::render()
{
    MSG current_message;
    if( PeekMessage( &current_message, nullptr, 0, 0, PM_REMOVE ) ) {
        if( current_message.message == WM_QUIT ) {
            return false;
        }

        TranslateMessage( &current_message );
        DispatchMessage( &current_message );
    }
    return true;
}

Overlay::Overlay_t Overlay::New( const EDeviceType device_type )
{
    switch( device_type ) {
    case EDeviceType::Direct2D:
        return std::make_unique<Device2D>();
    case EDeviceType::Direct3D9:
        return std::make_unique<Device3D9>();
    default:
        break;
    }
    return nullptr;
}

Overlay::RenderCallback* Overlay::add_callback(
    const std::string& name,
    RenderCallbackFn   callback,
    const bool         active )
{
    if( name.empty() || !callback ) {
        return nullptr;
    }
    if( m_RenderCallbacks.count( name ) != 0 ) {
        return nullptr;
    }

    m_RenderCallbacks.insert(
        std::make_pair(
            name,
            std::make_tuple(
                callback,
                active
            )
        )
    );

    return &m_RenderCallbacks.at( name );
}

bool Overlay::pause_callback(
    const std::string& name )
{
    if( !m_RenderCallbacks.count( name ) ) {
        return false;
    }

    std::get<1>( m_RenderCallbacks.at( name ) ) = false;
    return true;
}

bool Overlay::remove_callback(
    const std::string& name )
{
    if( !m_RenderCallbacks.count( name ) ) {
        return false;
    }

    m_RenderCallbacks.erase( name );
    return true;
}

bool Overlay::resume_callback(
    const std::string& name )
{
    if( !m_RenderCallbacks.count( name ) ) {
        return false;
    }

    std::get<1>( m_RenderCallbacks.at( name ) ) = true;
    return true;
}

void Overlay::scale_overlay()
{      
    auto fix_var = []( int32_t& in, int32_t& fix )
    {
        if( in == 0 ) {
            --in;
            ++fix;
        }
    };

    auto window_props = get_window_props( m_WndTarget );
    m_Width  = window_props.at( 2 );
    m_Height = window_props.at( 3 );    

    /// if we are on point 0, increase size and decrease position
    /// usefull for games like black ops 2
    fix_var( window_props.at( 0 ), m_Width );
    fix_var( window_props.at( 1 ), m_Height );

    MoveWindow(
        m_WndOverlay,
        window_props.at( 0 ),
        window_props.at( 1 ),
        m_Width,
        m_Height,
        TRUE
    );
}

void Overlay::shutdown()
{
#if defined(_UNICODE)
    UnregisterClass( convert_string_to_wide( m_ClassName ).c_str(), nullptr );
#else
    UnregisterClass( m_ClassName.c_str(), nullptr );
#endif
    if( m_WndOverlay ) {
        DestroyWindow( m_WndOverlay );
        m_WndOverlay = nullptr;
    }
    m_WndTarget = nullptr;
}

void Overlay::execute_callbacks()
{
    for( const auto& kp : m_RenderCallbacks ) {
        if( !std::get<1>( kp.second ) ) {
            continue;
        }
        std::get<0>( kp.second )( m_Surface.get() );
    }
}

LRESULT Overlay::window_procedure(
    HWND   window,
    UINT   message,
    WPARAM wparam,
    LPARAM lparam )
{
    switch( message ) {
    case WM_DESTROY:
        PostQuitMessage( EXIT_SUCCESS );
        return 0;
    case WM_KEYDOWN:
        return 0;
    case WM_ERASEBKGND:
        SendMessage( window, WM_PAINT, 0, 0 );
        return TRUE;
    case WM_PAINT:
        return 0;
    default:
        break;
    }
    return DefWindowProc( 
        window,
        message,
        wparam,
        lparam
    );
}

Overlay::WindowProps Overlay::get_window_props( HWND window )
{
    static WindowProps props;
    if( window ) {
        static POINT difference;
        static RECT rect_client, rect_window;
        difference = { 0, 0 };


        GetClientRect( window, &rect_client );
        props.at( 2 ) = rect_client.right;
        props.at( 3 ) = rect_client.bottom;


        GetWindowRect( window, &rect_window );
        difference = { 0, 0 };

        ClientToScreen( window, &difference );
        props.at( 0 ) = rect_window.left + ( difference.x - rect_window.left );
        props.at( 1 ) = rect_window.top + ( difference.y - rect_window.top );
    }
    return props;
}

std::string Overlay::generate_random_string( const size_t length )
{
    static std::string alpha_numeric_chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 gen( rd() );
    std::uniform_int_distribution<size_t> dis( 0U, alpha_numeric_chars.size() - 1U );

    std::string random_string( "", length );
    for( size_t i = 0; i < length; ++i ) {
        random_string.at( i ) = alpha_numeric_chars.at( dis( gen ) );
    }

    return random_string;
}

std::wstring Overlay::convert_string_to_wide( const std::string& str )
{
    static std::wstring_convert<
        std::codecvt_utf8_utf16<wchar_t>
    > converter;

    return converter.from_bytes( str );
}
