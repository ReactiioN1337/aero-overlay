#pragma once
#ifndef REACTIION_AERO_OVERLAY_OVERLAY_HPP
#define REACTIION_AERO_OVERLAY_OVERLAY_HPP
#include "../stdafx.hpp"

namespace render {

class Overlay
{
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
    /// Begins a scene.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    virtual bool            begin_scene();

    ///-------------------------------------------------------------------------------------------------
    /// Ends a scene.
    ///
    /// @author ReactiioN
    /// @date   04.03.2017
    ///-------------------------------------------------------------------------------------------------
    virtual void            end_scene() = 0;

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
};

}

#endif
