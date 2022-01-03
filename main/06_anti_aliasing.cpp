#pragma once

#include "color.h"
#include <array>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <memory>
#include <stb_image_write.h>

#include "image.h"
#include "ray.h"
#include "vec3.h"

#include <fmt/format.h>

#include "camera.h"
#include "random_generator.h"
#include "sphere.h"
#include "timer.h"
#include "world.h"

class SimpleWorld : public ray::World
{
public:
    void ClosestHit(const ray::Ray& ray, const ray::Sphere& sphere, const ray::HitRecord& hitRecord) const override
    {

    }
};

int main() {
    ray::Timer timer;
    ray::Random random;
    // Image
    constexpr float aspect_ratio = 16.0f / 9.0f;
    constexpr int image_height = 480;
    constexpr int image_width = static_cast<int>(image_height * aspect_ratio);
    // Camera
    const ray::Camera camera{ aspect_ratio };
    constexpr int samplePerPixel = 16;
    // World
    SimpleWorld world;
    world.Add({ ray::Vec3f(0,0,-1), 0.5f });
    world.Add({ ray::Vec3f(0,-100.5f,-1), 100.0f });
    // Render
    const auto image = std::make_unique<ray::Image<image_width, image_height>>();


    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            ray::Vec3f pixel_color;
            for (int s = 0; s < samplePerPixel; s++)
            {
                const auto u = (static_cast<float>(i) + random.Range(0.0f, 1.0f)) / static_cast<float>(image_width - 1);
                const auto v = (static_cast<float>(j) + random.Range(0.0f, 1.0f)) / static_cast<float>(image_height - 1);
                const ray::Ray r = camera.GetRay(u, v);

                const auto unit_direction = r.GetDir().GetNormalized();
                const auto t = 0.5f * (unit_direction.GetY() + 1.0f);
                ray::HitRecord hitRecord;
                const auto hit = world.Hit(r, 0.0f, std::numeric_limits<float>::max(), hitRecord);
                const auto color_f = hit ?
                    (hitRecord.normal + ray::One) * 0.5f :
                    (1.0f - t) * ray::One + t * ray::Vec3f(0.5f, 0.7f, 1.0f);
                pixel_color += color_f;
                //fmt::print("X: {} Y: {} T: {}, Color: {},{},{} \n", unit_direction.GetX(), unit_direction.GetY(), t, color_f.GetX(), color_f.GetY(), color_f.GetZ());
            }
            pixel_color = pixel_color * (1.0f / samplePerPixel);
            auto& color = image->GetColor(i, j);
            color = pixel_color.ToColor();
        }
    }
    const auto timeToRender = timer.Restart();
    image->WritePng("output_06.png");
    const auto timeToWriteToFile = timer.Restart();
    fmt::print("Time To Render: {}, Time To Write To File: {}", timeToRender.count(), timeToWriteToFile.count());
}
