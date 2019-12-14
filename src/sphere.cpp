#include <sphere.h>

std::optional<float> Sphere::Intersect(Vec3f origin, Vec3f dir) const {
    auto co = center - origin;
    const auto tca = Vec3f::Dot(co, dir);
    if (tca < 0) return {};
    const float d2 = (dir * tca - co).GetSquareMagnitude();
    const float r2 = radius * radius;
    if (d2 > r2) return {};
    const auto oc2 = co.GetSquareMagnitude();
    const auto t1 = Vec3f::Dot(dir, origin - center);
    const auto disc = t1 * t1 - oc2 - r2;
    const auto x1 = -t1 - std::sqrt(disc);
    const auto x2 = -t1 + std::sqrt(disc);
    return x1 > 0 && x1 < x2 ? x1 : x2;
}
