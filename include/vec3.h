#pragma once

#include <cmath>

namespace ray
{
    class Vec3f
    {
    public:
        constexpr Vec3f() noexcept = default;
        constexpr Vec3f(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

        [[nodiscard]] constexpr auto GetX() const noexcept { return x; }
        [[nodiscard]] constexpr auto GetY() const noexcept { return y; }
        [[nodiscard]] constexpr auto GetZ() const noexcept { return z; }

        constexpr auto operator+(Vec3f other) const noexcept { return Vec3f(x + other.x, y + other.y, z + other.z); }
        constexpr auto operator-(Vec3f other) const noexcept { return Vec3f(x - other.x, y - other.y, z - other.z); }
        constexpr auto operator*(float other) const noexcept { return Vec3f(x * other, y * other, z * other); }
        constexpr auto operator/(float other) const noexcept { return Vec3f(x / other, y / other, z / other); }

        constexpr auto& operator+=(Vec3f other) noexcept
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr auto& operator-=(Vec3f other) noexcept
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        constexpr auto& operator*=(float other) noexcept
        {
            x *= other;
            y *= other;
            z *= other;
            return *this;
        }

        constexpr auto& operator/=(float other) noexcept
        {
            x /= other;
            y /= other;
            z /= other;
            return *this;
        }

        [[nodiscard]] constexpr auto GetSquaredLength() const noexcept { return x * x + y * y + z * z; }
        [[nodiscard]] auto GetLength() const noexcept { return std::sqrt(GetSquaredLength()); }
        [[nodiscard]] constexpr auto GetNormalized() const noexcept { return (*this) / GetLength(); }

        static auto Dot(Vec3f v1, Vec3f v2) noexcept { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
        static auto Cross(Vec3f v1, Vec3f v2) noexcept
        {
            return Vec3f(
                v1.y * v2.z - v2.y * v1.z,
                v2.x * v1.z - v2.z * v1.x,
                v1.x * v2.y - v2.x * v1.y
            );
        }
    private:
        float x = 0.0f, y = 0.0f, z = 0.0f;
    };

    constexpr auto operator*(float number, Vec3f v) { return v * number; }

    static constexpr Vec3f One = Vec3f(1.0f, 1.0f, 1.0f);
    static constexpr Vec3f Right = Vec3f(1, 0, 0);
} // namespace ray
