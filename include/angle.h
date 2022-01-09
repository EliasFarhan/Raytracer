#pragma once


#include<cmath>

#   define UNIT_LIB_DEFAULT_TYPE float
#define UNIT_LIB_DISABLE_IOSTREAM true

#include <units.h>
using radian_t = units::unit_t<units::angle::radian, float>;
using degree_t = units::unit_t<units::angle::degree, float>;

inline float Sin(radian_t angle)
{
    return std::sin(angle.value());
}

inline float Cos(radian_t angle)
{
    return std::cos(angle.value());
}

inline float Tan(radian_t angle)
{
    return std::tan(angle.value());
}

