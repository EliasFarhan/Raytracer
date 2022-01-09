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

Metallic::Metallic(const Vec3f& albedo, float fuzz) : albedo_(albedo), fuzz_(fuzz)
{
}

bool Metallic::Scatter(const Ray& rayIn, const HitRecord& hitRecord, Vec3f& attenuation, Ray& scattered) const
{

    auto& random = Random::GetInstance();

    const Vec3f reflected = Vec3f::Reflect(rayIn.GetDir(), hitRecord.normal);
    scattered = Ray(hitRecord.point, reflected+fuzz_*Sphere::RandomInUnitSphere(random));
    attenuation = albedo_;
    return (Vec3f::Dot(scattered.GetDir(), hitRecord.normal) > 0);
}

Dielectric::Dielectric(float refractionIndex) : refractionIndex_(refractionIndex)
{
}

bool Dielectric::Scatter(const Ray& rayIn, const HitRecord& hitRecord, Vec3f& attenuation, Ray& scattered) const
{
    auto& random = Random::GetInstance();
    attenuation = One;
    const float ratio = hitRecord.frontFace ? 1.0f / refractionIndex_ : refractionIndex_;
    const auto dir = rayIn.GetDir();
    const auto cosTheta = std::min(Vec3f::Dot(-dir, hitRecord.normal), 1.0f);
    const auto sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
    const auto cannotRefract = ratio * sinTheta > 1.0f;
    const auto rayOutDir = cannotRefract || Reflectance(cosTheta, ratio) > random.Range(0.0f,1.0f) ? 
        Vec3f::Reflect(dir, hitRecord.normal) :
        Vec3f::Refract(dir, hitRecord.normal, ratio);
    scattered = ray::Ray(hitRecord.point, rayOutDir);
    return true;
}

float Dielectric::Reflectance(float cosine, float refIndex)
{
    // Use Schlick's approximation for reflectance.
    auto r0 = (1.0f - refIndex) / (1.0f + refIndex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
}
} // namespace ray
