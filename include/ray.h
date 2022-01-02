#pragma once
#include "vec3.h"

namespace ray
{
    class Ray
    {
    public:
        constexpr Ray() : dir_(Right) {}
        constexpr Ray(Vec3f origin, Vec3f dir) : origin_(origin), dir_(dir){}

        [[nodiscard]] constexpr Vec3f GetOrigin() const noexcept { return origin_; }
        [[nodiscard]] constexpr Vec3f GetDir() const noexcept { return dir_; }
        [[nodiscard]] constexpr Vec3f At(float t) const noexcept { return origin_ + t * dir_; }
    private:
        Vec3f origin_;
        Vec3f dir_;
    };
} // namespace ray
