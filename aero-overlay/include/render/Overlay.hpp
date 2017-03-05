#pragma once
#ifndef REACTIION_AERO_OVERLAY_OVERLAY_HPP
#define REACTIION_AERO_OVERLAY_OVERLAY_HPP
#include "Surface.hpp"

namespace render {

enum class EDeviceType
{
    ///< Direct2D + Direct Draw
    Direct2D = 0,

    ///< DirectX 9
    Direct3D9,
};

class Overlay
{
    /// An alias for the unique surface pointer.
    using Surface_t       = std::unique_ptr<Surface>;
    /// An alias for the unique overlay pointer.
    using Overlay_t       = std::unique_ptr<Overlay>;
    /// An alias for the window properties.
    ///< 0: x position
    ///< 1: y position
    ///< 2: width
    ///< 3: height
    using WindowProps     = std::array<int32_t, 4>;

public:
    ///-------------------------------------------------------------------------------------------------
    /// Default constructor.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///-------------------------------------------------------------------------------------------------
    Overlay();

    ///-------------------------------------------------------------------------------------------------
    /// Destructor.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///-------------------------------------------------------------------------------------------------
    virtual ~Overlay();

    ///-------------------------------------------------------------------------------------------------
    /// Creates a new bool.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  target_window_title Target window title.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    virtual bool            create(
        const std::string& target_window_title );

    ///-------------------------------------------------------------------------------------------------
    /// Determines if we can in foreground.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    virtual bool            in_foreground() const;

    ///-------------------------------------------------------------------------------------------------
    /// Renders this object.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    virtual bool            render();

    ///-------------------------------------------------------------------------------------------------
    /// Gets the surface.
    ///
    /// @author ReactiioN
    /// @date   05.03.2017
    ///
    /// @return Null if it fails, else the surface.
    ///-------------------------------------------------------------------------------------------------
    inline Surface*         get_surface() const;

    ///-------------------------------------------------------------------------------------------------
    /// News the given device type.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  device_type Type of the device.
    ///
    /// @return An Overlay_t.
    ///-------------------------------------------------------------------------------------------------
    static Overlay_t        New( const EDeviceType device_type );

public:
    ///-------------------------------------------------------------------------------------------------
    /// Scales the overlay size to the target window size.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///-------------------------------------------------------------------------------------------------
    virtual void            scale_overlay();

    ///-------------------------------------------------------------------------------------------------
    /// Shuts down this object and frees any resources it is using.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///-------------------------------------------------------------------------------------------------
    virtual void            shutdown();

protected:
    ///-------------------------------------------------------------------------------------------------
    /// Window procedure.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  window  Handle of the window.
    /// @param  message The message.
    /// @param  wparam  The wParam field of the message.
    /// @param  lparam  The lParam field of the message.
    ///
    /// @return A CALLBACK.
    ///-------------------------------------------------------------------------------------------------
    static LRESULT CALLBACK window_procedure(
        HWND               window,
        UINT               message,
        WPARAM             wparam,
        LPARAM             lparam );

    ///-------------------------------------------------------------------------------------------------
    /// Gets window properties.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  window  Handle of the window.
    ///
    /// @return The window properties.
    ///-------------------------------------------------------------------------------------------------
    static WindowProps      get_window_props( 
        HWND               window );
public:
    ///-------------------------------------------------------------------------------------------------
    /// Generates a random string.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  length  The length.
    ///
    /// @return The random string.
    ///-------------------------------------------------------------------------------------------------
    static std::string      generate_random_string( 
        const size_t       length );

    ///-------------------------------------------------------------------------------------------------
    /// Convert string to wide.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @param  str The string.
    ///
    /// @return The string converted to wide.
    ///-------------------------------------------------------------------------------------------------
    static std::wstring     convert_string_to_wide( 
        const std::string& str );

protected:
    HWND            m_WndOverlay = nullptr;
    HWND            m_WndTarget  = nullptr;
    int32_t         m_Width      = 0;
    int32_t         m_Height     = 0;
    std::string     m_ClassName;
    std::string     m_WindowTitle;
    Surface_t       m_Surface;
};

inline Surface* Overlay::get_surface() const
{
    return m_Surface ? m_Surface.get() : nullptr;
}

}

#endif
