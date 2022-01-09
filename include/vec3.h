#pragma once

#include <cmath>
#include <color.h>

#include "random_generator.h"
#include "ray.h"
#include "ray.h"

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
        constexpr auto operator-() const noexcept { return Vec3f(-x, -y, -z); }
        constexpr auto operator*(Vec3f other) const noexcept { return Vec3f(x * other.x, y * other.y, z * other.z); }
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
        [[nodiscard]] constexpr bool NearZero() const { return GetSquaredLength() < 0.001f; }
        static constexpr auto Dot(Vec3f v1, Vec3f v2) noexcept { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

        static Vec3f Reflect(Vec3f dir, Vec3f normal);
        static Vec3f Refract(Vec3f uv, Vec3f normal, float ratio);

        static constexpr auto Cross(Vec3f v1, Vec3f v2) noexcept
        {
            return Vec3f(
                v1.y * v2.z - v2.y * v1.z,
                v2.x * v1.z - v2.z * v1.x,
                v1.x * v2.y - v2.x * v1.y
            );
        }

        [[nodiscard]] constexpr Color ToColor() const noexcept
        {
            Color color;
            color.r = static_cast<std::uint8_t>(x * 255.999f);
            color.g = static_cast<std::uint8_t>(y * 255.999f);
            color.b = static_cast<std::uint8_t>(z * 255.999f);
            return color;
        }

        static Vec3f Random(ray::Random& random)
        {
            return {random.Range(0.0f, 1.0f), random.Range(0.0f, 1.0f), random.Range(0.0f, 1.0f)};
        }
        static Vec3f Random(ray::Random& random, float min, float max)
        {
            return {random.Range(min, max), random.Range(min, max), random.Range(min, max)};
        }
    private:
        float x = 0.0f, y = 0.0f, z = 0.0f;
    };



    constexpr auto operator*(float number, Vec3f v) { return v * number; }

    inline Vec3f Vec3f::Reflect(Vec3f dir, Vec3f normal)
    {
        return dir - 2.0f * Dot(dir, normal) * normal;
    }

    inline Vec3f Vec3f::Refract(Vec3f uv, Vec3f normal, float ratio)
    {
        const auto cosTheta = std::min(Dot(-uv, normal), 1.0f);
        const auto rOutPerp = ratio * (uv + cosTheta * normal);
        const auto rOutParallel = -std::sqrt(std::abs(1.0f - rOutPerp.GetSquaredLength())) * normal;
        return rOutPerp + rOutParallel;

    }

    static constexpr Vec3f One = Vec3f(1.0f, 1.0f, 1.0f);
    static constexpr Vec3f Right = Vec3f(1, 0, 0);
} // namespace ray
