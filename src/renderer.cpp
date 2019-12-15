#include <renderer.h>
#include <chrono>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>

void Renderer::Render(const Scene &scene) {
    frameBuffer_.clear();
    const auto screenResolution = renderConfig_.screenResolution;
    const auto[width, height] = screenResolution;
    const auto invWidth = 1.0f / float(width), invHeight = 1.0f / float(height);
    const auto aspect = width / float(height);
    float angle = tan(M_PI * 0.5 * renderConfig_.fov / 180.);
    frameBuffer_.resize(renderConfig_.screenResolution.first * renderConfig_.screenResolution.second);
    auto start = std::chrono::steady_clock::now();
    size_t px = 0;
    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            float xx = (2 * ((x + 0.5f) * invWidth) - 1) * angle * aspect;
            float yy = (1 - 2 * ((y + 0.5f) * invHeight)) * angle;
            Vec3f raydir(xx, yy, -1);
            frameBuffer_[px] = Trace(Vec3f(), raydir, scene, 0);
            px++;
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
        pxData[i * 3] = frameBuffer_[i].x;
        pxData[i * 3 + 1] = frameBuffer_[i].y;
        pxData[i * 3 + 2] = frameBuffer_[i].z;
    }
    stbi_write_bmp(renderConfig_.bmpOutput.c_str(), renderConfig_.screenResolution.first,
                   renderConfig_.screenResolution.second, 3, &pxData[0]);
}

Color Renderer::Trace(Vec3f origin, Vec3f dir, const Scene &scene, int currentDepth) {

    float dnear = INFINITY;
    const Sphere *closestSphere = nullptr;
    for (const auto &sphere : scene.spheres_) {
        const auto interesectDist = sphere.Intersect(origin, dir);
        if (interesectDist.has_value() && interesectDist < dnear) {
            dnear = interesectDist.value();
            closestSphere = &sphere;
        }
    }
    if (closestSphere == nullptr) return renderConfig_.defaultColor;

    const size_t index = closestSphere - &scene.spheres_[0];
    const auto &material = scene.materials_[index];
    const Sphere &sphere = *closestSphere;
    Color surfaceColor;
    const auto hitPos = origin + dir * dnear;
    const auto normalTmp = (hitPos - sphere.center);
    const auto hitNormal = normalTmp / normalTmp.GetMagnitude();
    const float bias = 1e-4;
    if (material.reflection and currentDepth < renderConfig_.MAX_RAY_DEPTH) {
        const auto tmpReflDir = dir - hitNormal * 2.0f * Vec3f::Dot(dir, hitNormal);
        const auto reflDir = tmpReflDir / tmpReflDir.GetMagnitude();
        surfaceColor += Trace(hitPos-hitNormal*bias, reflDir, scene, currentDepth + 1);
    } else {
        for (size_t i = 0; i < scene.spheres_.size(); ++i) {
            if (scene.materials_[i].emissionColor.x > 0) {
                // this is a light
                float transmission = 1.0f;
                Vec3f lightDir = scene.spheres_[i].center - hitPos;
                lightDir = lightDir / lightDir.GetMagnitude();
                for (unsigned j = 0; j < scene.spheres_.size(); ++j) {
                    if (i != j) {
                        auto interectDist = scene.spheres_[j].Intersect(hitPos + hitNormal * bias,
                                                                        lightDir);
                        if (interectDist.has_value()) {
                            transmission = 0.0f;
                            break;
                        }
                    }
                }
                surfaceColor += material.baseColor * transmission *
                                std::max(float(0), Vec3f::Dot(hitNormal, lightDir)) *
                                scene.materials_[i].emissionColor;
            }
        }
    }

    return surfaceColor + material.emissionColor;
}

void Renderer::SetConfig(const RenderConfig &config) {
    renderConfig_ = config;

}
