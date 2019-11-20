///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2019, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once
#include "detail/requirements.hpp"

namespace aero {

class overlay final
{
public:
    overlay();

    overlay(
        const overlay& ) = delete;

    overlay(
        overlay&& rhs ) noexcept;

    ~overlay();

    overlay& operator = (
        const overlay& ) = delete;

    overlay& operator = (
        overlay&& rhs ) noexcept;

    bool attach(
        std::string_view window_title );

    bool attach(
        std::uint32_t process_id );

    bool attach(
        HWND target_window );

    void destroy();

    _NODISCARD
    bool message_loop() const noexcept;

    void scale();

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

private:
    static std::intptr_t __stdcall window_proc(
        void*          window_handle,
        std::uint32_t  message,
        std::uintptr_t wparam,
        std::intptr_t  lparam );

private:
    std::string   _class;
    std::string   _title;
    HWND          _window = nullptr;
    HWND          _target = nullptr;
    std::uint32_t _width = 0;
    std::uint32_t _height = 0;
};

}
