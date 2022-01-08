#include "color.h"
#include <array>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <memory>
#include <stb_image_write.h>

#include "image.h"
#include "ray.h"
#include "vec3.h"

#include <fmt/format.h>

#include "sphere.h"
#include "timer.h"
#include "world.h"

class SimpleLambertianWorld : public ray::World
{
public:
    void ClosestHit(const ray::Ray& ray, const ray::Sphere& sphere, const ray::HitRecord& hitRecord) const override
    {
        
    }
};

int main() {
    ray::Timer timer;
    // Image
    constexpr float aspect_ratio = 16.0f / 9.0f;
    constexpr int image_height = 480;
    constexpr int image_width = static_cast<int>(image_height * aspect_ratio);
    // Camera

    constexpr auto viewport_height = 2.0f;
    constexpr auto viewport_width = aspect_ratio * viewport_height;
    constexpr auto focal_length = 1.0f;

    constexpr auto origin = ray::Vec3f();
    constexpr auto horizontal = ray::Vec3f(viewport_width, 0, 0);
    constexpr auto vertical = ray::Vec3f(0, viewport_height, 0);
    constexpr auto lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - ray::Vec3f(0, 0, focal_length);
    // World
    SimpleLambertianWorld world;
    world.Add({ ray::Vec3f(0,0,-1), 0.5f },);
    world.Add({ ray::Vec3f(0,-100.5f,-1), 100.0f },);
    // Render
    const auto image = std::make_unique<ray::Image<image_width, image_height>>();
    

    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            const auto u = static_cast<float>(i) / static_cast<float>(image_width - 1);
            const auto v = static_cast<float>(j) / static_cast<float>(image_height - 1);
            const ray::Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            
            const auto unit_direction = r.GetDir().GetNormalized();
            const auto t = 0.5f * (unit_direction.GetY() + 1.0f);
            ray::HitRecord hitRecord;
            const auto hit = world.Hit(r, 0.0f, std::numeric_limits<float>::max(), hitRecord);
            const auto color_f = hit ? 
                (hitRecord.normal + ray::One)*0.5f:
                (1.0f - t) * ray::One + t * ray::Vec3f(0.5f, 0.7f, 1.0f);
            //fmt::print("X: {} Y: {} T: {}, Color: {},{},{} \n", unit_direction.GetX(), unit_direction.GetY(), t, color_f.GetX(), color_f.GetY(), color_f.GetZ());

            auto& color = image->GetColor(i, j);
            color = color_f.ToColor();
        }
    }
    const auto timeToRender = timer.Restart();
    image->WritePng("output_05.png");
    const auto timeToWriteToFile = timer.Restart();
    fmt::print("Time To Render: {}, Time To Write To File: {}", timeToRender.count(), timeToWriteToFile.count());
}
