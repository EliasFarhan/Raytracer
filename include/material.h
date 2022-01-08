#pragma once

#include "hit.h"
#include "ray.h"
#include "vec3.h"

namespace ray
{
    class Material
    {
    public:
        virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Vec3f& attenuation, Ray& scattered) const = 0;
    };

class Lambertian : public Material
{
public:
    Lambertian(const Vec3f& albedo);
    bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Vec3f& attenuation, Ray& scattered) const override;
private:
    Vec3f albedo_;
};

class Metallic : public Material
{
public:
    Metallic(const Vec3f& albedo);
    bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Vec3f& attenuation, Ray& scattered) const override;
private:
    Vec3f albedo_;
};

} // namespace ray

