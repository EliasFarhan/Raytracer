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

#include <omp.h>

static ray::Random* globalRandom = nullptr;

class SimpleMaterialWorld : public ray::World
{
public:
    void ClosestHit(const ray::Ray& ray, const ray::Sphere& sphere, const ray::HitRecord& hitRecord,
        ray::Payload& payload) const override
    {
        if (payload.depth == MAX_DEPTH_RECURSION)
        {
            payload.color = ray::Vec3f();
            return;
        }

        const auto sphereIndex = std::distance(spheres_.data(), &sphere);
        const auto* material = materials_[sphereIndex].get();
        ray::Ray newRay;
        ray::Vec3f attenuation;
        if(material->Scatter(ray, hitRecord, attenuation, newRay))
        {
            ray::Payload newPayload;
            newPayload.depth = payload.depth + 1;
            ray::HitRecord newHitRecord;
            RayCast(newRay, 0.001f, std::numeric_limits<float>::max(), newHitRecord, newPayload);
            payload.color = attenuation * newPayload.color;
        }
        else
        {
            payload.color = ray::Vec3f();
        }
        
    }

    void MissHit(const ray::Ray& ray, ray::Payload& payload) const override
    {
        const auto unit_direction = ray.GetDir().GetNormalized();
        const auto t = 0.5f * (unit_direction.GetY() + 1.0f);
        payload.color = (1.0f - t) * ray::One + t * ray::Vec3f(0.5f, 0.7f, 1.0f);
    }
private:
    static constexpr int MAX_DEPTH_RECURSION = 50;
};

int main() {
    ray::Timer timer;
    ray::Random random;
    globalRandom = &random;
    // Image
    constexpr float aspect_ratio = 16.0f / 9.0f;
    constexpr int image_height = 480;
    constexpr int image_width = static_cast<int>(image_height * aspect_ratio);
    // Camera
    const ray::Camera camera{ aspect_ratio };
    constexpr int samplePerPixel = 50;
    // World
    SimpleMaterialWorld world;
    world.Add({ ray::Vec3f(0,-100.5f,-1), 100.0f }, std::make_unique<ray::Lambertian>(ray::Vec3f(0.8f, 0.8f, 0.0f)));
    world.Add({ ray::Vec3f(0,0,-1), 0.5f }, std::make_unique<ray::Lambertian>(ray::Vec3f(0.7f,0.3f, 0.3f)));
    world.Add({ ray::Vec3f(-1,0,-1), 0.5f }, std::make_unique<ray::Metallic>(ray::Vec3f(0.8f, 0.8f, 0.8f), 0.3f));
    world.Add({ ray::Vec3f(1,0,-1), 0.5f }, std::make_unique<ray::Dielectric>(1.5f));
    world.Add({ ray::Vec3f(1,0,-1), -0.4f }, std::make_unique<ray::Dielectric>(1.5f));
    // Render
    const auto image = std::make_unique<ray::Image<image_width, image_height>>();

    #pragma omp parallel for
    for (int j = 0; j < image_height; ++j) {
        #pragma omp parallel for
        for (int i = 0; i < image_width; ++i) {
            ray::Vec3f pixel_color;
            for (int s = 0; s < samplePerPixel; s++)
            {
                const auto u = (static_cast<float>(i) + random.Range(0.0f, 1.0f)) / static_cast<float>(image_width - 1);
                const auto v = (static_cast<float>(j) + random.Range(0.0f, 1.0f)) / static_cast<float>(image_height - 1);
                const ray::Ray r = camera.GetRay(u, v);


                ray::HitRecord hitRecord;
                ray::Payload payload;
                world.RayCast(r, 0.001f, std::numeric_limits<float>::max(), hitRecord, payload);
                const auto color_f = payload.color;
                pixel_color += color_f;
                //fmt::print("X: {} Y: {} T: {}, Color: {},{},{} \n", unit_direction.GetX(), unit_direction.GetY(), t, color_f.GetX(), color_f.GetY(), color_f.GetZ());
            }
            pixel_color = pixel_color * (1.0f / samplePerPixel);
            // Gamma correction
            pixel_color = ray::Vec3f(std::sqrt(pixel_color.GetX()), std::sqrt(pixel_color.GetY()), std::sqrt(pixel_color.GetZ()));
            auto& color = image->GetColor(i, j);
            color = pixel_color.ToColor();
        }
    }
    const auto timeToRender = timer.Restart();
    image->WritePng("output_09.png");
    const auto timeToWriteToFile = timer.Restart();
    fmt::print("Time To Render: {}, Time To Write To File: {}", timeToRender.count(), timeToWriteToFile.count());
}
