#include <sphere.h>

std::optional<float> Sphere::Intersect(Vec3f origin, Vec3f dir) const {
    auto co = center - origin;
    const auto tca = Vec3f::Dot(co, dir);
    if (tca < 0) return {};
    const float d2 = (dir * tca - co).GetSquareMagnitude();
    const float r2 = radius * radius;
    if (d2 > r2) return {};
    const auto oc2 = co.GetSquareMagnitude();
    const auto disc = tca * tca - oc2 + r2;
    const auto thc = std::sqrt(disc);
    const auto x1 = tca - thc;
    const auto x2 = tca + thc;
    return x1 > 0 and x1 < x2 ? x1 : x2;
}
