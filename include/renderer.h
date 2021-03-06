#pragma once

#include <utility>
#include "scene.h"

struct RenderConfig {
    std::pair<unsigned, unsigned> screenResolution = {1024, 1024};
    float fov = 30.0f;
    unsigned MAX_RAY_DEPTH = 5;
    Color defaultColor = Color(1.0f, 1.0f, 1.0f);
    std::string pngOutput = "out.png";
};

class Renderer {
public:
    void Render(const Scene &scene, bool threaded);

    void Export();

    void SetConfig(const RenderConfig &config);

private:
    Color Trace(Vec3f origin, Vec3f dir, const Scene &scene, int currentDepth);

    std::vector<Color> frameBuffer_;
    RenderConfig renderConfig_;
};