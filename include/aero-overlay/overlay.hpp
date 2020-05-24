///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include <aero-overlay/surface.hpp>

namespace aero {

class overlay final
{
public:
    overlay();

    overlay(
        const overlay&
    ) = delete;

    overlay(
        overlay&& rhs
    ) noexcept;

    ~overlay();

    overlay& operator = (
        const overlay&
    ) = delete;

    overlay& operator = (
        overlay&& rhs
    ) noexcept;

    api_status attach(
        std::string_view window_title
    );

    api_status attach(
        std::uint32_t process_id
    );

    api_status attach(
        HWND target_window
    );

    void destroy();

    _NODISCARD
    bool message_loop() const noexcept;

    void scale();

    void set_surface(
        surface_ptr surface
    );

    _NODISCARD __forceinline
    const std::string& get_class_name() const noexcept
    {
        return _class;
    }

    _NODISCARD __forceinline
    const std::string& get_title() const noexcept
    {
        return _title;
    }

    _NODISCARD
    surface_ptr get_surface() const noexcept
    {
        return _surface;
    }

private:
    static std::intptr_t __stdcall window_proc(
        void*          window_handle,
        std::uint32_t  message,
        std::uintptr_t wparam,
        std::intptr_t  lparam
    );

private:
    std::string   _class;
    std::string   _title;
    HWND          _window = nullptr;
    HWND          _target = nullptr;
    std::uint32_t _width  = 0;
    std::uint32_t _height = 0;
    surface_ptr   _surface;
};

}
