#include "world.h"

namespace ray
{
bool World::Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const
{
    HitRecord tmpHitRecord;
    const Sphere* closestSphere = nullptr;
    for(const auto& sphere: spheres_)
    {
        if(sphere.Hit(ray, t_min, t_max, tmpHitRecord))
        {
            if(tmpHitRecord.t < hit_record.t)
            {
                hit_record = tmpHitRecord;
                closestSphere = &sphere;
            }
        }
    }
    if(closestSphere != nullptr)
    {
        ClosestHit(ray, *closestSphere, hit_record);
        return true;
    }
    return false;
}

void World::Clear()
{
    spheres_.clear();
}

void World::Add(const Sphere& sphere)
{
    spheres_.push_back(sphere);
}
} // namespace ray
