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
    fmt::print("Starting rendering!\n");
    ray::Timer timer;
    ray::Random random;
    // Image
    constexpr float aspect_ratio = 16.0f / 9.0f;
    constexpr int image_height = 360;
    constexpr int image_width = static_cast<int>(image_height * aspect_ratio);
    // Camera
    ray::Vec3f lookfrom(13.0f, 2.0f, 3.0f);
    ray::Vec3f lookat(0, 0, 0);
    ray::Vec3f vup(0, 1, 0);
    constexpr auto dist_to_focus = 10.0f;
    constexpr auto aperture = 0.1f;
    const ray::Camera camera{ lookfrom, lookat, vup, 20.0f,  aspect_ratio };// , aperture, dist_to_focus };
    constexpr int samplePerPixel = 100;
    // World
    SimpleMaterialWorld world;
    auto groundMaterial = std::make_unique<ray::Lambertian>(ray::Vec3f(0.5f, 0.5f, 0.5f));
    world.Add({ ray::Vec3f(0,1000,0), 1000.0f }, std::move(groundMaterial));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            const auto choose_mat = random.Range(0.0f, 1.0f);
            ray::Vec3f center(a + 0.9f * random.Range(0.0f, 1.0f), 0.2f, b + 0.9f * random.Range(0.0f, 1.0f));

            if ((center - ray::Vec3f(4, 0.2f, 0)).GetLength() > 0.9f) {
                std::unique_ptr<ray::Material> sphere_material;

                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo =
                        ray::Vec3f(random.Range(0.f, 1.0f), random.Range(0.f, 1.0f), random.Range(0.f, 1.0f)) *
                        ray::Vec3f(random.Range(0.f, 1.0f), random.Range(0.f, 1.0f), random.Range(0.f, 1.0f));
                    sphere_material = std::make_unique<ray::Lambertian>(albedo);
                    world.Add({ center, 0.2f }, std::move(sphere_material));
                }
                else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = ray::Vec3f(random.Range(0.5f, 1.0f), random.Range(0.5f, 1.0f), random.Range(0.5f, 1.0f));
                    auto fuzz = random.Range(0.0f, 0.5f);
                    sphere_material = std::make_unique<ray::Metallic>(albedo, fuzz);
                    world.Add({ center, 0.2f }, std::move(sphere_material));
                }
                else {
                    // glass
                    sphere_material = std::make_unique<ray::Dielectric>(1.5f);
                    world.Add({ center, 0.2f }, std::move(sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_unique<ray::Dielectric>(1.5f);
    world.Add({ ray::Vec3f(0, 1, 0), 1.0f }, std::move(material1));

    auto material2 = std::make_unique<ray::Lambertian>(ray::Vec3f{ 0.4f, 0.2f, 0.1f });
    world.Add({ ray::Vec3f(-4, 1, 0), 1.0f }, std::move(material2));

    auto material3 = std::make_unique<ray::Metallic>(ray::Vec3f(0.7f, 0.6f, 0.5f), 0.0f);
    world.Add({ ray::Vec3f(4, 1, 0), 1.0f }, std::move(material3));
    // Render
    const auto image = std::make_unique<ray::Image<image_width, image_height>>();
    omp_set_dynamic(0);     // Explicitly disable dynamic teams
    omp_set_num_threads(6); // Use 6 threads for all consecutive parallel regions
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
    image->WritePng("output_11.png");
    const auto timeToWriteToFile = timer.Restart();
    fmt::print("Time To Render: {}s, Time To Write To File: {}s", timeToRender.count(), timeToWriteToFile.count());
}
