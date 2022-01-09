#pragma once
#include "hit.h"
#include "ray.h"
#include "vec3.h"

namespace ray
{
    class Sphere : public HittableInterface
    {
    public:
        constexpr Sphere(Vec3f center, float radius) : center_(center), radius_(radius) {}
        [[nodiscard]] constexpr Vec3f GetCenter() const noexcept { return center_; }
        [[nodiscard]] constexpr float GetRadius() const noexcept { return radius_; }

        [[nodiscard]] constexpr float Hit(const Ray& ray) const noexcept
        {
            const Vec3f oc = ray.GetOrigin() - center_;
            const auto a = Vec3f::Dot(ray.GetDir(), ray.GetDir());
            const auto b = 2.0f * Vec3f::Dot(oc, ray.GetDir());
            const auto c = Vec3f::Dot(oc, oc) - radius_ * radius_;
            const auto discriminant = b * b - 4.0f * a * c;
            
            return discriminant > 0.0f ? (-b - std::sqrt(discriminant)) / (2.0f * a): -1.0f;
        }

        bool Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override
        {
            const Vec3f oc = ray.GetOrigin() - center_;
            const auto a = Vec3f::Dot(ray.GetDir(), ray.GetDir());
            const auto b = 2.0f * Vec3f::Dot(oc, ray.GetDir());
            const auto c = Vec3f::Dot(oc, oc) - radius_ * radius_;
            const auto discriminant = b * b - 4.0f * a * c;
            if (discriminant < 0.0f) [[unlikely]]
            {
                return false;
            }
            const auto sqrtd = std::sqrt(discriminant);
            auto root = (-b - sqrtd) / (2.0f * a);
            if (root < t_min || t_max < root)
            {
                root = (-b + sqrtd) / (2.0f * a);
                if (root < t_min || t_max < root)
                {
                    return false;
                }
            }

            hit_record.t = root;
            hit_record.point = ray.At(root);
            const auto outwardNormal = (hit_record.point - center_) / radius_;
            hit_record.frontFace = Vec3f::Dot(ray.GetDir(), outwardNormal) < 0.0f;
            hit_record.normal = hit_record.frontFace ? outwardNormal : -outwardNormal;
            return true;
        }

        static Vec3f RandomInUnitSphere(Random& random)
        {
            while(true)
            {
                const auto p = Vec3f::Random(random, -1.0f, 1.0f);
                if(p.GetSquaredLength() >= 1) continue;
                return p;
            }
        }
        static Vec3f RandomUnitVector(Random& random)
        {
            return RandomInUnitSphere(random).GetNormalized();
        }
    private:
        Vec3f center_;
        float radius_ = 0.0f;
    };
    
} // namespace ray
