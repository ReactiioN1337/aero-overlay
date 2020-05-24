# aero-overlay

A transparent window which is located above a target window to have the possibility to draw over it.
My initial code can be found in [1.0 branch](https://github.com/ReactiioN1337/aero-overlay/tree/1.0),
development takes place in `develop` branch. Do **not** push into the `master` branch.

## Installation

> I'm using `/MT` in **release** and `/MTd` in **debug** as runtime library. So either adjust your project settings to mine or vice versa!

- Clone the repository into your solution directory (I alway do this in `contrib`)
- Right click on your solution
- Add > Existing Project ... > and select the `aero-overlay.vcxproj`
- Right click on your project in your solution
- Add > Reference... > and select `aero-overlay`
- Right click on your project in your solution > Properties
- C/C++ > General > Additional Include Directories and add the `include` directory of this repository!

## Usage

A basic overlay can be created using the title, process id or window handle.

```C++
#include <aero-overlay/overlay.hpp>

std::int32_t main(
    const std::int32_t argc,
    const char**       argv
)
{
    if( argc < 2 ) {
        printf( R"(usage:
    executable.exe "SuperCoolGameTitle"
)" );
        return -1;
    }

    const auto overlay = std::make_unique<aero::overlay>();
    const auto status  = overlay->attach( argv[ 1 ] );

    if( status != aero::api_status::success ) {
        printf( "[>] failed to create overlay: %d\n", status );
        return -1;
    }

    const auto surface = overlay->get_surface();
    const auto font    = surface->add_font( "test", "Verdana", 12.f );

    surface->add_callback([&surface, &font]
    {
        surface->text( 5.f, 5.f, font, 0xFFFFFFFF, "this is an example" );
    } );

    while( overlay->message_loop() ) {
        if( surface->begin_scene() ) {
            surface->end_scene();
        }

        std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
    }
};
```

> Callbacks are only executed if the target window is in foreground

## Custom rendering support

With `overlay::set_surface()` you can use your own components. You only need to re-implement the classes `font` and `surface`.

> This function **must be called** before `attach()`!

```C++
overlay->set_surface( std::make_share<my_surface>() );

const auto status = overlay->attach( ... );
```
