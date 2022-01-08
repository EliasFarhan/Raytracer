#include "material.h"

#include "sphere.h"
#include "random_generator.h"

namespace ray
{
Lambertian::Lambertian(const Vec3f& albedo): albedo_(albedo)
{
    
}

bool Lambertian::Scatter(const Ray& rayIn, const HitRecord& hitRecord, Vec3f& attenuation, Ray& scattered) const
{
    auto& random = Random::GetInstance();
    auto scatter_direction = hitRecord.normal + Sphere::RandomUnitVector(random);

    // Catch degenerate scatter direction
    if (scatter_direction.NearZero())
        scatter_direction = hitRecord.normal;

    scattered = Ray(hitRecord.point, scatter_direction);
    attenuation = albedo_;
    return true;
}

Metallic::Metallic(const Vec3f& albedo) : albedo_(albedo)
{
}

bool Metallic::Scatter(const Ray& rayIn, const HitRecord& hitRecord, Vec3f& attenuation, Ray& scattered) const
{

    auto& random = Random::GetInstance();
    const Vec3f reflected = Vec3f::Reflect(rayIn.GetDir(), hitRecord.normal);
    scattered = Ray(hitRecord.point, reflected);
    attenuation = albedo_;
    return (Vec3f::Dot(scattered.GetDir(), hitRecord.normal) > 0);
}
} // namespace ray
