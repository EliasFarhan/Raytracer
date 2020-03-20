#include <renderer.h>
#include <chrono>
#include <iostream>
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <thread>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void Renderer::Render(const Scene &scene, bool threaded) {
    frameBuffer_.clear();
    const auto screenResolution = renderConfig_.screenResolution;
    const auto[width, height] = screenResolution;
    const auto invWidth = 1.0f / float(width), invHeight = 1.0f / float(height);
    const auto aspect = width / float(height);
    float angle = std::tan(float(M_PI) * 0.5f * renderConfig_.fov / 180.0f);
    frameBuffer_.resize(renderConfig_.screenResolution.first * renderConfig_.screenResolution.second);
    auto start = std::chrono::steady_clock::now();
    if(threaded) {
        std::array<std::thread, 4> threadPool;
        auto func = [&](Vec2<unsigned> start, Vec2<unsigned> end) {
            for (unsigned y = start.y; y < end.y; y++) {
                for (unsigned x = start.x; x < end.x; x++) {
                    const size_t px = y + x * renderConfig_.screenResolution.second;
                    float xx = (2 * ((x + 0.5f) * invWidth) - 1) * angle * aspect;
                    float yy = (1 - 2 * ((y + 0.5f) * invHeight)) * angle;
                    Vec3f raydir(xx, yy, -1);
                    frameBuffer_[px] = Trace(Vec3f(), raydir, scene, 0);
                }
            }
        };
        threadPool[0] = std::thread(func, Vec2<unsigned>(0, 0), Vec2<unsigned>(width / 2, height / 2));
        threadPool[1] = std::thread(func, Vec2<unsigned>(width / 2, 0), Vec2<unsigned>(width, height / 2));
        threadPool[2] = std::thread(func, Vec2<unsigned>(0, height / 2), Vec2<unsigned>(width / 2, height));
        threadPool[3] = std::thread(func, Vec2<unsigned>(width / 2, height / 2), Vec2<unsigned>(width, height));

        for (auto &t : threadPool) {
            t.join();
        }
    } else
    {
        for (unsigned y = 0; y < height; y++) {
            for (unsigned x = 0; x < width; x++) {
                const size_t px = y + x * renderConfig_.screenResolution.second;
                float xx = (2 * ((x + 0.5f) * invWidth) - 1) * angle * aspect;
                float yy = (1 - 2 * ((y + 0.5f) * invHeight)) * angle;
                Vec3f raydir(xx, yy, -1);
                frameBuffer_[px] = Trace(Vec3f(), raydir, scene, 0);
            }
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed = end - start;
    std::cout << "Time elapsed: " << elapsed.count() << '\n' <<
              std::chrono::duration_cast<std::chrono::duration<double>>(elapsed).count() << "s\n";

}

void Renderer::Export() {
    std::vector<char> pxData(frameBuffer_.size() * 3);
    for (size_t i = 0; i < frameBuffer_.size(); i++) {
        const float gamma = 2.2f;
        Color hdrColor = frameBuffer_[i];
        //reinhard tonemapping
        Color mapped = hdrColor / (hdrColor + Vec3f::One);
        //gamma correction
        mapped = Color::Pow(mapped, 1.0f/gamma);
        pxData[i * 3] = std::uint8_t (mapped.x*255);
        pxData[i * 3 + 1] = std::uint8_t (mapped.y*255);
        pxData[i * 3 + 2] = std::uint8_t (mapped.z*255);
    }
    stbi_write_png(renderConfig_.pngOutput.c_str(), renderConfig_.screenResolution.first,
                   renderConfig_.screenResolution.second, 3, &pxData[0], 0);
}

float mix(float a, float b, float mix) {
    return b * mix + a * (1.0f - mix);
}

Color Renderer::Trace(Vec3f origin, Vec3f dir, const Scene &scene, int currentDepth) {

    float closestDist = INFINITY;
    const Sphere *closestSphere = nullptr;
    for (const auto &sphere : scene.spheres_) {
        const auto interesectDist = sphere.Intersect(origin, dir);
        if (interesectDist.has_value() && interesectDist < closestDist) {
            closestDist = interesectDist.value();
            closestSphere = &sphere;
        }
    }
    if (closestSphere == nullptr) return renderConfig_.defaultColor;

    const size_t index = closestSphere - &scene.spheres_[0];
    const auto &material = scene.materials_[index];
    const Sphere &sphere = *closestSphere;

    Color surfaceColor;
    const auto hitPos = origin + dir * closestDist;
    const auto normalTmp = (hitPos - sphere.center);
    const auto hitNormal = normalTmp / normalTmp.GetMagnitude();
    const float bias = 1e-4;
    if (material.reflection && currentDepth < renderConfig_.MAX_RAY_DEPTH) {
        const auto facingRatio = -Vec3f::Dot(dir, hitNormal);
        const auto fresnelEffect = mix(pow(1.0f - facingRatio, 3.0f), 1.0f, 0.1f);
        const auto tmpReflDir = dir - hitNormal * 2.0f * Vec3f::Dot(dir, hitNormal);
        const auto reflDir = tmpReflDir / tmpReflDir.GetMagnitude();
        const auto reflectionColor = Trace(hitPos + hitNormal * bias, reflDir, scene, currentDepth + 1);

        surfaceColor += Color(Vec3f(reflectionColor) * fresnelEffect);
    } else {
        for (size_t i = 0; i < scene.spheres_.size(); ++i) {
            if (scene.materials_[i].emissionColor.x > 0 || scene.materials_[i].emissionColor.y > 0 ||
                scene.materials_[i].emissionColor.z > 0) {
                // this is a light
                float transmission = 1.0f;
                Vec3f lightDir = scene.spheres_[i].center - hitPos;
                lightDir = lightDir / lightDir.GetMagnitude();
                for (unsigned j = 0; j < scene.spheres_.size(); ++j) {
                    if (i != j) {
                        const auto interectDist = scene.spheres_[j].Intersect(hitPos + hitNormal * bias,
                                                                              lightDir);
                        if (interectDist.has_value()) {
                            transmission = 0.0f;
                            break;
                        }
                    }
                }
                surfaceColor += Color(Vec3f(material.baseColor) * transmission *
                                std::max(0.0f, Vec3f::Dot(hitNormal, lightDir)) *
                                Vec3f(scene.materials_[i].emissionColor));
            }
        }
    }

    return surfaceColor + material.emissionColor;
}

void Renderer::SetConfig(const RenderConfig &config) {
    renderConfig_ = config;

}
