#pragma once

#include <vector>
#include <string_view>
#include <sphere.h>
#include <material.h>

struct Scene
{
    std::vector<Sphere> spheres_;
    std::vector<Material> materials_;

    void GenerateRandomScene();
    void LoadSceneFromJson(const std::string_view path);
};

