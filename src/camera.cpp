#include "camera.h"

namespace ray
{
Camera::Camera(float aspectRatio)
{
    constexpr auto viewport_height = 2.0f;
    const auto viewport_width = aspectRatio * viewport_height;
    constexpr auto focal_length = 1.0f;

    origin_ = Vec3f(0, 0, 0);
    horizontal_ = Vec3f(viewport_width, 0.0, 0.0);
    vertical_ = Vec3f(0.0, viewport_height, 0.0);
    lowerLeftCorner_ = origin_ - horizontal_ / 2.0f - vertical_ / 2.0f - Vec3f(0, 0, focal_length);
}

Ray Camera::GetRay(float u, float v) const
{
    return {origin_, (lowerLeftCorner_ + u * horizontal_ + v * vertical_ - origin_).GetNormalized()};
}
} // namespace ray
