///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#include <aero-overlay/overlay.hpp>
#include <dwmapi.h>
#include <random>
#include "direct2d/d2d_surface.hpp"
using namespace aero;

template<typename type = RECT>
type get_window_props(
    HWND hwnd
)
{
    RECT client{}, window{};
    GetClientRect( hwnd, &client );
    GetWindowRect( hwnd, &window );

    POINT diff{};
    ClientToScreen( hwnd, &diff );

    return {
        window.left + ( diff.x - window.left ),
        window.top  + ( diff.y - window.top ),
        client.right,
        client.bottom
    };
}

overlay::overlay()
{
    const auto random_string = []( const std::size_t length )
    {
        constexpr static char charset[] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
        };
        constexpr static std::size_t num_chars = sizeof charset;

        static std::random_device rd;
        std::mt19937 gen( rd() );
        const std::uniform_int_distribution<std::size_t> engine(
            std::numeric_limits<std::size_t>::min(),
            num_chars - 1
        );

        std::string str( length, '\0' );
        for( auto& c : str ) {
            c = charset[ engine( gen ) ];
        }

        return str;
    };

    _class = random_string( 32 );
    _title = random_string( 64 );
}

overlay::overlay(
    overlay&& rhs
) noexcept
{
    *this = std::move( rhs );
}

overlay::~overlay()
{
    destroy();
}

overlay& overlay::operator = (
    overlay&& rhs
) noexcept
{
    _class  = std::move( rhs._class );
    _title  = std::move( rhs._title );
    _window = rhs._window;
    _target = rhs._target;
    _width  = rhs._width;
    _height = rhs._height;

    rhs._window = nullptr;
    rhs._target = nullptr;
    rhs._width  = 0;
    rhs._height = 0;

    return *this;
}

api_status overlay::attach(
    const std::string_view window_title
)
{
    return window_title.empty()
        ? api_status::missing_window_title
        : attach( FindWindowA( nullptr, window_title.data() ) );
}

api_status overlay::attach(
    const std::uint32_t process_id
)
{
    using callback_data_type = std::pair<const std::uint32_t*, HWND>;

    auto data = std::make_pair( &process_id, static_cast<HWND>( nullptr ) );

    EnumWindows(
        []( HWND hwnd, const LPARAM lparam ) -> std::int32_t
        {
            if( !hwnd ) {
                return 1;
            }

            DWORD pid = 0;
            GetWindowThreadProcessId( hwnd, &pid );

            auto& data = *reinterpret_cast<callback_data_type*>( lparam );
            if( *data.first == pid ) {
                data.second = hwnd;
                return 0;
            }
            return 1;
        },
        reinterpret_cast<LPARAM>( &data )
    );

    return attach( data.second );
}

api_status overlay::attach(
    HWND target_window
)
{
    if( !target_window ) {
        return api_status::missing_window_handle;
    }

    auto dwm_enabled = 0;
    if( FAILED( DwmIsCompositionEnabled( &dwm_enabled ) ) || !dwm_enabled ) {
        return api_status::missing_aero_feature;
    }

    WNDCLASSEXA wc = {
        sizeof( WNDCLASSEX ),
        0,
        reinterpret_cast<WNDPROC>( window_proc ),
        0,
        0,
        GetModuleHandleA( nullptr ),
        LoadIconA( nullptr, MAKEINTRESOURCEA( 32512 ) ),
        LoadCursorA( nullptr, MAKEINTRESOURCEA( 32512 ) ),
        nullptr,
        nullptr,
        _class.data(),
        LoadIconA( nullptr, MAKEINTRESOURCEA( 32512 ) ),
    };

    if( !RegisterClassExA( &wc ) ) {
        return api_status::failed_to_register_window;
    }

    _target = target_window;
    _window = CreateWindowExA(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED /* | WS_EX_TOOLWINDOW | WS_EX_COMPOSITED*/,
        _class.data(),
        _title.data(),
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        nullptr,
        nullptr,
        GetModuleHandleA( nullptr ),
        this
    );
    if( !_window ) {
        return api_status::failed_to_create_window;
    }

    scale();
    if( !SetLayeredWindowAttributes( _window, RGB( 0, 0, 0 ), 255, LWA_ALPHA ) ) {
        return api_status::failed_to_make_window_transparent;
    }

    const auto margins = get_window_props<MARGINS>( _window );
    if( FAILED( DwmExtendFrameIntoClientArea( _window, &margins ) ) ) {
        return api_status::failed_to_make_window_transparent;
    }

    ShowWindow( _window, SW_SHOWDEFAULT );
    UpdateWindow( _window );

    if( !_surface ) {
        set_surface( std::make_shared<d2d_surface>() );
    }

    return _surface->initialize( _window, _target );
}

void overlay::destroy()
{
    if( !_class.empty() ) {
        UnregisterClassA( _class.data(), nullptr );
    }
    if( _window ) {
        DestroyWindow( _window );
    }

    _window = nullptr;
    _target = nullptr;

    if( _surface ) {
        _surface->release();
    }
}

bool overlay::message_loop() const noexcept
{
    if( !_window ) {
        return false;
    }

    MSG msg{};
    if( PeekMessageA( &msg, nullptr, 0, 0, PM_REMOVE ) > 0 ) {
        TranslateMessage( &msg );
        DispatchMessageA( &msg );

        if( msg.message == WM_QUIT ) {
            return false;
        }
    }

    return true;
}

void overlay::scale()
{
    static auto fix = []( long& in, std::uint32_t& out )
    {
        if( in == 0 ) {
            in++;
            out--;
        }
        else {
            in--;
            out++;
        }
    };

    auto props = get_window_props( _target );

    _width  = static_cast<std::uint32_t>( props.right );
    _height = static_cast<std::uint32_t>( props.bottom );

    fix( props.left, _width );
    fix( props.top, _height );

    MoveWindow(
        _window,
        props.left,
        props.top,
        static_cast<std::int32_t>( _width ),
        static_cast<std::int32_t>( _height ),
        1
    );
}

void overlay::set_surface(
    surface_ptr surface
)
{
    if( _surface ) {
        _surface->release();
    }

    _surface = std::move( surface );
}

std::intptr_t overlay::window_proc(
    void*                window_handle,
    const std::uint32_t  message,
    const std::uintptr_t wparam,
    const std::intptr_t  lparam
)
{
    auto* const hwnd = static_cast<HWND>( window_handle );
    switch( message ) {
    case WM_DESTROY:
        PostQuitMessage( EXIT_SUCCESS );
        return 0;
    case WM_KEYDOWN:
        return 0;
    case WM_ERASEBKGND:
        SendMessageA( hwnd, WM_PAINT, 0, 0 );
        return TRUE;
    default:
        break;
    }
    return DefWindowProcA( hwnd, message, wparam, lparam );
}
