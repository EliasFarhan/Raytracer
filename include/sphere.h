#pragma once

#include <vector.h>
#include <optional>
#include <primitive.h>

struct Sphere
{
    Vec3f center;
    float radius = 0.0f;

    std::optional<float> Intersect(Vec3f origin, Vec3f dir) const;


};