#pragma once
#include "Types.h"

namespace Plasmium
{
    class vec3
    {
    public:
        union {
            struct { float x, y, z; };
            struct { float r, g, b; };
        };

        vec3() : x(0), y(0), z(0) {}
        vec3(float val) : x(val), y(val), z(val) {}
        vec3(float x, float y, float z) : x(x), y(y), z(z) {}

        vec3& operator=(const vec3& rhs);
        vec3& operator+=(const vec3& rhs);
        vec3& operator-=(const vec3& rhs);

        vec3& operator*=(float scalar);
        vec3& operator/=(float scalar);

        float& operator[](uint32 index);

        vec3 Normalize() const;
        float Length() const;
        float LengthSquared() const;
        float Dot(const vec3& rhs) const;
        vec3 Cross(const vec3& rhs) const;
    };

    vec3 operator+(const vec3& lhs, const vec3& rhs);
    vec3 operator-(const vec3& lhs, const vec3& rhs);

    vec3 operator*(const vec3& v, float scalar);
    vec3 operator*(float scalar, const vec3& v);
    vec3 operator/(const vec3& v, float scalar);
    vec3 operator/(float scalar, const vec3& v);

    bool operator==(const vec3& lhs, const vec3& rhs);
    bool operator!=(const vec3& lhs, const vec3& rhs);
}