#pragma once
#include "ray.h"
#include "vec3.h"

namespace ray
{
    class Camera
    {
    public:
        Camera(float aspectRatio);

        [[nodiscard]] Ray GetRay(float u, float v) const;
    private:
        Vec3f origin_;
        Vec3f lowerLeftCorner_;
        Vec3f horizontal_;
        Vec3f vertical_;
    };
} // namespace ray
