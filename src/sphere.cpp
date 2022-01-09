#include "sphere.h"

namespace ray
{
    bool Sphere::Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const
    {
        const Vec3f oc = ray.GetOrigin() - center_;
        const auto a = Vec3f::Dot(ray.GetDir(), ray.GetDir());
        const auto b = 2.0f * Vec3f::Dot(oc, ray.GetDir());
        const auto c = Vec3f::Dot(oc, oc) - radius_ * radius_;
        const auto discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f)
        {
            return false;
        }
        const auto sqrtd = std::sqrt(discriminant);
        auto root = (-b - sqrtd) / (2.0f * a);
        if(root < t_min || t_max < root)
        {
            root = (-b + sqrtd) / (2.0f * a);
            if( root < t_min || t_max < root)
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
} // namespace ray
