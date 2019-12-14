#pragma once

#include <vector.h>
struct Material
{
    Vec3f baseColor, emissionColor;
    float reflection = 0.0f;//0 means no reflection, 1 is full reflection
};