#include "world.h"

namespace ray
{
bool World::RayCast(const Ray& ray, float t_min, float t_max, HitRecord& hit_record, Payload& payload) const
{
    HitRecord tmpHitRecord;
    const Sphere* closestSphere = nullptr;
    for(const auto& sphere: spheres_)
    {
        if(sphere.Hit(ray, t_min, t_max, tmpHitRecord)) [[unlikely]]
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
        ClosestHit(ray, *closestSphere, hit_record, payload);
        return true;
    }
    MissHit(ray, payload);
    return false;
}

void World::Clear()
{
    spheres_.clear();
}

void World::Add(const Sphere& sphere, std::unique_ptr<Material> material)
{
    spheres_.push_back(sphere);
    materials_.push_back(std::move(material));
}
} // namespace ray
