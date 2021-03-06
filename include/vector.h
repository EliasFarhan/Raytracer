#pragma once
/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <ostream>
#include <glm/glm.hpp>
#include <array>
#include <cmath>
template<typename T>
class Vec2
{
public:

    Vec2() : x(0), y(0)
    {
    }

    explicit Vec2(T same) : x(same), y(same)
    {

    }

    Vec2(T X, T Y) : x(X), y(Y)
    {
    }

    template<typename U>
    explicit Vec2(const Vec2<U>& vector) : x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y))
    {
    }

    T GetSquareMagnitude()
    { return x * x + y * y; }

    static T Dot(const Vec2<T>& v1, const Vec2<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    Vec2<T> operator+(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x + rhs.x, y + rhs.y);
    }

    Vec2<T>& operator+=(const Vec2<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    Vec2<T> operator-(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x - rhs.x, y - rhs.y);
    }

    Vec2<T>& operator-=(const Vec2<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    Vec2<T> operator*(T rhs) const
    {
        return Vec2<T>(x * rhs, y * rhs);
    }

    Vec2<T> operator*(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x * rhs.x, y * rhs.y);
    }


    Vec2<T>& operator*=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }

    Vec2<T> operator/(T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    Vec2<T>& operator/=(T rhs)
    {
        this->x /= rhs;
        this->y /= rhs;
        return *this;
    }

    bool operator==(const Vec2<T>& right)
    {
        return x == right.x && y == right.y;
    }

    bool operator!=(const Vec2<T>& right)
    {
        return !(*this == right);
    }

    T GetMagnitude() const;

    Vec2<T> Normalized() const
    {
        return (*this) / (*this).GetMagnitude();
    }

    Vec2<T> Rotate(T angle) const;

    static Vec2<T> Lerp(const Vec2<T>& v1, const Vec2<T>& v2, T t)
    {
        return v1 + (v2 - v1) * t;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& dt)
    {
        os << "Vec2(" << dt.x << "," << dt.y << ")";
        return os;
    }

    template<typename U>
    explicit Vec2(const U& v);

    template<typename U>
    explicit operator U() const;

    template<typename U = float>
    static U AngleBetween(const Vec2& v1, const Vec2& v2);
    T x; ///< X coordinate of the vector
    T y; ///< Y coordinate of the vector

    const static Vec2 Zero;
    const static Vec2 One;
};

template<typename T>
Vec2<T> operator*(T lhs, const Vec2<T>& rhs)
{
    return Vec2<T>(rhs.x * lhs, rhs.y * lhs);
}

using Vec2f = Vec2<float>;


template<>
inline float Vec2f::GetMagnitude() const
{
    return sqrtf(x * x + y * y);
}

template<>
inline Vec2<float> Vec2f::Rotate(float angle) const
{
    const float radianAngle = glm::radians(angle);
    return Vec2f(cosf(radianAngle) * x - sinf(radianAngle) * y,
                 sinf(radianAngle) * x + cosf(radianAngle) * y);
}




template<typename T>
class alignas(4* sizeof(T)) Vec3
{
public:
    T x, y, z;
    const static Vec3 Zero;
    const static Vec3 One;

    Vec3() : x(0), y(0), z(0)
    {
    }

    explicit Vec3(T same) : x(same), y(same), z(same)
    {

    }

    Vec3(T X, T Y, T Z) : x(X), y(Y), z(Z)
    {

    }

    template<typename U>
    explicit
    Vec3(const Vec3<U>& vector) : x(static_cast<T>(vector.x)),
                                  y(static_cast<T>(vector.y)),
                                  z(static_cast<T>(vector.z))
    {
    }

    T GetSquareMagnitude() const
    { return Dot(*this, *this); }


    template<typename ReturnT = float>
    ReturnT GetMagnitude() const
    {
        return std::sqrt(GetSquareMagnitude());
    }

    static Vec3<T> Pow(Vec3<T> base, T exponent)
    {
        return Vec3(std::pow(base.x, exponent), std::pow(base.y, exponent), std::pow(base.z, exponent));
    }

    static T Dot(Vec3<T> v1, Vec3<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    Vec3<T> operator+(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vec3<T>& operator+=(const Vec3<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }


    Vec3<T> operator-(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vec3<T>& operator-=(const Vec3<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }

    Vec3<T> operator*(T rhs) const
    {
        return Vec3<T>(x * rhs, y * rhs, z * rhs);
    }

    Vec3<T> operator*(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
    }


    Vec3<T>& operator*=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }

    Vec3<T> operator/(T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    Vec3<T>& operator/=(T rhs)
    {
        *this = *this / rhs;
        return *this;
    }
    Vec3<T> operator/(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
    }
    bool operator==(const Vec3<T>& right) const
    {
        return x == right.x && y == right.y && z == right.z;
    }

    bool operator!=(const Vec3<T>& right) const
    {
        return !(*this == right);
    }
    template<typename U = float>
    static U AngleBetween(const Vec3& v1, const Vec3& v2);

};

using Vec3f = Vec3<float>;




template<typename T>
class alignas(4* sizeof(T)) Vec4
{
public:
    T x, y, z, w;
    const static Vec4 Zero;
    const static Vec4 One;

    Vec4()noexcept : x(0), y(0), z(0), w(0)
    {
    }

    explicit Vec4(T same) : x(same), y(same), z(same), w(same)
    {

    }

    Vec4(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W)
    {

    }

    template<typename U>
    explicit Vec4(const Vec4<U>& vector) : x(static_cast<T>(vector.x)),
                                           y(static_cast<T>(vector.y)),
                                           z(static_cast<T>(vector.z)),
                                           w(static_cast<T>(vector.w))
    {
    }

    T GetSquareMagnitude() const
    { return Dot(*this, *this); }


    template<typename ReturnT = float>
    ReturnT GetMagnitude() const
    {
        return std::sqrt(GetSquareMagnitude());
    }

    static T Dot(const Vec4<T> v1, const Vec4<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    static T Dot3(const Vec4<T> v1, const Vec4<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    Vec4<T> operator+(const Vec4<T>& rhs) const
    {
        return Vec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vec4<T>& operator+=(const Vec4<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        this->w += rhs.w;
        return *this;
    }

    Vec4<T> operator-(const Vec4<T>& rhs) const
    {
        return Vec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vec4<T>& operator-=(const Vec4<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        this->w -= rhs.w;
        return *this;
    }

    Vec4<T> operator*(T rhs) const
    {
        return Vec4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
    }

    Vec4<T> operator*(const Vec4<T>& rhs) const
    {
        return Vec4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }


    Vec4<T>& operator*=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        this->w *= rhs;
        return *this;
    }

    Vec4<T> operator/(T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    Vec4<T>& operator/=(T rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    bool operator==(const Vec4<T>& right) const
    {
        return x == right.x && y == right.y && z == right.z && w == right.w;
    }

    bool operator!=(const Vec4<T>& right) const
    {
        return !(*this == right);
    }


};

using Vec4f = Vec4<float>;

template<typename T, int N>
struct alignas(N*sizeof(T)) NVec4
{
    std::array<T, N> xs;
    std::array<T, N> ys;
    std::array<T, N> zs;
    std::array<T, N> ws;
    NVec4():xs{}, ys{}, zs{}, ws{} {};
    explicit NVec4(const std::array<Vec4<T>, N> soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = soaV[i].w;
        }
    }
    explicit NVec4(const Vec3<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = 0.0f;
        }
    }
    explicit NVec4(const Vec4<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = soaV[i].w;
        }
    }

    std::array<T, N> GetMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i] + ws[i] * ws[i];
            result[i] = std::sqrt(result[i]);
        }
        return result;
    }

};




using FourVec4f = NVec4<float, 4>;
using EightVec4f = NVec4<float, 8>;

