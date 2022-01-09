#include "camera.h"

#include "angle.h"
#include "sphere.h"

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

    Camera::Camera(Vec3f origin, Vec3f lookAt, Vec3f up, float fovy, float aspectRatio, float aperture, float focusDist) : origin_(origin),
        lensRadius_(aperture / 2.0f)
    {

        const radian_t theta = degree_t(fovy);
        const auto h = Tan(theta / 2.0f);
        const auto viewport_height = 2.0f * h;
        const auto viewport_width = aspectRatio * viewport_height;

        w_ = (origin_ - lookAt).GetNormalized();
        u_ = Vec3f::Cross(up, w_).GetNormalized();
        v_ = Vec3f::Cross(w_, u_);


        horizontal_ = u_ * viewport_width;
        vertical_ = viewport_height * v_;
        lowerLeftCorner_ = origin_ - horizontal_ / 2.0f - vertical_ / 2.0f - w_;
    }

    Ray Camera::GetRay(float s, float t) const
    {

        auto& random = Random::GetInstance();
        const auto rd = lensRadius_ * Sphere::RandomInUnitSphere(random);
        const auto offset = u_ * rd.GetX() + v_ * rd.GetY();
        return { origin_ + offset, (lowerLeftCorner_ + s * horizontal_ + t * vertical_ - origin_ - offset).GetNormalized() };
    }
} // namespace ray
