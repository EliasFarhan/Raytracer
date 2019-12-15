#pragma once

#include <vector.h>
struct Material
{
    Color baseColor, emissionColor;
    bool reflection = false;//0 means no reflection, 1 is full reflection

};