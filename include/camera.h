#pragma once
#include "ray.h"
#include "vec3.h"

namespace ray
{
    class Camera
    {
    public:
        Camera(float aspectRatio);
        Camera(Vec3f origin, Vec3f lookAt, Vec3f up, float fovy, float aspect, float aperture=0.0f, float focusDist=1.0f);
        [[nodiscard]] Ray GetRay(float s, float t) const;
    private:
        Vec3f origin_;
        Vec3f lowerLeftCorner_;
        Vec3f horizontal_;
        Vec3f vertical_;
        float lensRadius_ = 0.0f;
        Vec3f u_;
        Vec3f v_;
        Vec3f w_;
    };
} // namespace ray
