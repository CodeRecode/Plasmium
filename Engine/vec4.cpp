#pragma once
#include "math.h"
#include "vec4.h"

namespace Plasmium
{
    vec4 vec4::operator=(const vec4& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
        return *this;
    }

    vec4 vec4::operator+=(const vec4& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    vec4 vec4::operator-=(const vec4& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    vec4 vec4::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    vec4 vec4::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    float& vec4::operator[](uint32 index)
    {
        return (&x)[index];
    }

    vec4 operator+(const vec4& lhs, const vec4& rhs)
    {
        return vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    vec4 operator-(const vec4& lhs, const vec4& rhs)
    {
        return vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }

    vec4 operator*(const vec4& v, float scalar)
    {
        return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
    }

    vec4 operator*(float scalar, const vec4& v)
    {
        return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
    }

    vec4 operator/(const vec4& v, float scalar)
    {
        return vec4(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
    }

    vec4 operator/(float scalar, const vec4& v)
    {
        return vec4(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
    }

    vec4 operator==(const vec4& lhs, const vec4& rhs)
    {
        return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w);
    }

    vec4 operator!=(const vec4& lhs, const vec4& rhs)
    {
        return (lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w);
    }

    vec4 normalize(const vec4& v)
    {
        return (v / length(v));
    }

    float length(const vec4& v)
    {
        return sqrtf(lengthSquared(v));
    }

    float lengthSquared(const vec4& v)
    {
        return (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    }

    float dot(const vec4& lhs, const vec4& rhs)
    {
        return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w);
    }
}