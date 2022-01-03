#pragma once
#include <vector>

#include "hit.h"
#include "sphere.h"

namespace ray
{
    class World : public HittableInterface
    {
    public:
        bool Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override;
        virtual void ClosestHit(const Ray& ray, const Sphere& sphere, const HitRecord& hitRecord) const = 0;
        void Clear();
        void Add(const Sphere& sphere);
    private:
        std::vector<Sphere> spheres_;
    };
} // namespace ray
