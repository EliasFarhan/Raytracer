#include <renderer.h>

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

    return Color();
}

void Renderer::SetConfig(const RenderConfig &config) {
    renderConfig_ = config;

}
