#pragma once
#include "ray.h"
#include "vec3.h"

namespace ray
{

struct HitRecord
{
    Vec3f point;
    Vec3f normal;
    float t = std::numeric_limits<float>::max();
    bool frontFace = false;
};

struct Payload
{
    Vec3f color;
    int depth = 0;
};

class HittableInterface
{
public:
    virtual ~HittableInterface() = default;
    virtual bool Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const = 0;
};

} // namespace ray
