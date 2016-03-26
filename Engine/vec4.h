#pragma once
#include "Types.h"

namespace Plasmium
{
    class vec4
    {
    public:
        union {
            struct { float x, y, z, w; };
            struct { float r, g, b, a; };
        };

        vec4() : x(0), y(0), z(0), w(0) { };
        vec4(float val) : x(val), y(val), z(val), w(val) { };
        vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { };

        vec4 operator=(const vec4& rhs);
        vec4 operator+=(const vec4& rhs);
        vec4 operator-=(const vec4& rhs);

        vec4 operator*=(float scalar);
        vec4 operator/=(float scalar);

        float& operator[](uint32 index);
    };

    vec4 operator+(const vec4& lhs, const vec4& rhs);
    vec4 operator-(const vec4& lhs, const vec4& rhs);

    vec4 operator*(const vec4& v, float scalar);
    vec4 operator*(float scalar, const vec4& v);
    vec4 operator/(const vec4& v, float scalar);
    vec4 operator/(float scalar, const vec4& v);

    vec4 operator==(const vec4& lhs, const vec4& rhs);
    vec4 operator!=(const vec4& lhs, const vec4& rhs);

    vec4 normalize(const vec4& v);
    float length(const vec4& v);
    float lengthSquared(const vec4& v);
    float dot(const vec4& lhs, const vec4& rhs);
}