#include <render/Device2D.hpp>
#include <render/Device3D9.hpp>
using namespace render;

std::unique_ptr<Overlay> create_overlay( const EDeviceType type, const std::string& window_title )
{
    auto overlay = Overlay::New( type );
    if( !overlay ) {
        return nullptr;
    }
    if( !overlay->create( window_title ) ) {
        return nullptr;
    }

    auto surface = overlay->get_surface();
    if( !surface ) {
        return nullptr;
    }

    const auto is_d3d9 = type == EDeviceType::Direct3D9;

    if( !surface->add_font(
        "default",
        "Segoe UI",
        is_d3d9 ? 14 : 12,
        is_d3d9 ? FW_NORMAL : DWRITE_FONT_WEIGHT_NORMAL,
        /// or DEFAULT_QUALITY instead of PROOF_QUALITY for anti aliasing
        is_d3d9 ? PROOF_QUALITY : DWRITE_FONT_STRETCH_NORMAL
    ) ) {
        return nullptr;
    }

    return std::move( overlay );
}

int32_t main( int32_t argc, char** argv )
{
    auto print_helper = []{
        std::cout << "usage: executable.exe --device \"d2d or d3d9\" --window \"window title\"" << std::endl;
        return EXIT_FAILURE;
    };    

    if( argc != 5 ) {
        return print_helper();
    }

    const std::string device_cmd( argv[ 1 ] );
    const std::string device_key( argv[ 2 ] );
    const std::string window_cmd( argv[ 3 ] );
    const std::string window_key( argv[ 4 ] );
    if( device_cmd.compare( "--device" ) != 0 ||
        window_cmd.compare( "--window" ) != 0 ) {
        return print_helper();
    }

    std::unique_ptr<Overlay> overlay = nullptr;
    if( !device_key.compare( "d2d" ) ) {
        overlay = create_overlay( EDeviceType::Direct2D, window_key );
    }
    else if( !device_key.compare( "d3d9" ) ) {
        overlay = create_overlay( EDeviceType::Direct3D9, window_key );
    }
    else {
        return print_helper();
    }

    auto* callback = overlay->add_callback(
        "test", []( Surface* surface ) {
            surface->text( 5, 5, "default", 0xFFFFFFFF, "Aero Overlay by ReactiioN" );
       }
    );
    if( !callback ) {
        return EXIT_FAILURE;
    }

    while( overlay->render() ) {
        
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    overlay->shutdown();
    return EXIT_SUCCESS;
}
