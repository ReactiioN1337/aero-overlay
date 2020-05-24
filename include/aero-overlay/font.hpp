///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include <aero-overlay/core/api_status.hpp>

namespace aero {
class font abstract
{
protected:
    font(
        const std::string&  family,
        const float         height,
        const std::uint32_t weight,
        const std::uint32_t flags
    )
        : _family( family )
        , _height( height )
        , _weight( weight )
        , _flags( flags )
    { }

public:
    virtual ~font() = default;

    virtual api_status create() = 0;

    virtual void release() = 0;

    virtual void** address_of() noexcept = 0;

    virtual void* get_data() const noexcept = 0;

    virtual void get_text_size(
        std::wstring_view text,
        float*            width,
        float*            height
    ) = 0;

    virtual void get_text_size(
        std::string_view text,
        float*           width,
        float*           height
    ) = 0;

    _NODISCARD
    std::string_view get_family() const noexcept
    {
        return _family;
    }

    _NODISCARD constexpr
    float get_height() const noexcept
    {
        return _height;
    }

    _NODISCARD constexpr
    std::uint32_t get_weight() const noexcept
    {
        return _weight;
    }

    _NODISCARD constexpr
    std::uint32_t get_flags() const noexcept
    {
        return _flags;
    }

    template<typename type>
    _NODISCARD
    type** address_of() noexcept
    {
        return reinterpret_cast<type**>( address_of() );
    }

    template<typename type>
    _NODISCARD
    type* get_data() const noexcept
    {
        return static_cast<type*>( get_data() );
    }
protected:
    std::string   _family;
    float         _height = 0.f;
    std::uint32_t _weight = 0;
    std::uint32_t _flags  = 0;
};

using font_ptr = std::shared_ptr<font>;
}
