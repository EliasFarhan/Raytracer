#pragma once
#include <memory>
#include <vector>

#include "hit.h"
#include "material.h"
#include "sphere.h"

namespace ray
{
    class World
    {
    public:
        virtual ~World() = default;
        bool RayCast(const Ray& ray, float t_min, float t_max, HitRecord& hit_record, Payload& payload) const;
        virtual void ClosestHit(const Ray& ray, const Sphere& sphere, const HitRecord& hitRecord, Payload& payload) const = 0;
        virtual void MissHit(const Ray& ray, Payload& payload) const = 0;
        void Clear();
        void Add(const Sphere& sphere, std::unique_ptr<Material> material = nullptr);
    protected:
        std::vector<Sphere> spheres_;
        std::vector<std::unique_ptr<Material>> materials_;
    };
} // namespace ray
