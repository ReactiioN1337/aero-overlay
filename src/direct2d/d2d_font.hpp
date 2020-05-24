///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include "pre_include.hpp"

namespace aero {
class d2d_font final
    : public font
{
public:
    d2d_font(
        IDWriteFactory*    factory,
        const std::string& family,
        float              height,
        std::uint32_t      weight,
        std::uint32_t      flags
    );

    ~d2d_font() override;

private:
    void kill();

public:
    api_status create() override;

    void release() override;

    void** address_of() noexcept override;

    _NODISCARD
    void* get_data() const noexcept override;

    void get_text_size(
        std::wstring_view text,
        float*            width,
        float*            height
    ) override;

    void get_text_size(
        std::string_view text,
        float*           width,
        float*           height
    ) override;

private:
    IDWriteFactory*    _factory = nullptr;
    IDWriteTextFormat* _data = nullptr;
};
}
