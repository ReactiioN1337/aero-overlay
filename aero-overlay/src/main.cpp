#include <render/Device2D.hpp>
#include <render/Device3D9.hpp>
using namespace render;

int32_t main()
{
    auto overlay = Overlay::New( EDeviceType::Direct2D );
    if( !overlay ) {
        return EXIT_FAILURE;
    }

    if( !overlay->create( "Counter-Strike: Global Offensive" ) ) {
        return EXIT_FAILURE;
    }

    while( overlay->render() ) {
        
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    overlay->shutdown();
    return EXIT_SUCCESS;
}
