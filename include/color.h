#pragma once

#include <cstdint>

#include "vec3.h"

namespace ray
{
    struct Color
    {
        std::uint8_t r = 0, g = 0, b = 0;
    };

    constexpr Color ToColor(Vec3f v) noexcept
    {
        Color color;
        color.r = static_cast<std::uint8_t>(v.GetX() * 255.999f);
        color.g = static_cast<std::uint8_t>(v.GetY() * 255.999f);
        color.b = static_cast<std::uint8_t>(v.GetZ() * 255.999f);
        return color;
    }
} // namespace ray
