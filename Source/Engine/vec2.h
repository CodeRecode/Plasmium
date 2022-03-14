#pragma once
#include "Types.h"

namespace Plasmium
{
    class vec2
    {
    public:
        float x, y;

        vec2() : x(0), y(0) {}
        vec2(float val) : x(val), y(val) {}
        vec2(float x, float y) : x(x), y(y) {}

        vec2& operator=(const vec2& rhs);
        vec2& operator+=(const vec2& rhs);
        vec2& operator-=(const vec2& rhs);

        vec2& operator*=(float scalar);
        vec2& operator/=(float scalar);

        float& operator[](uint32 index);

        vec2 Normalize() const;
        float Length() const;
        float LengthSquared() const;
        float Dot(const vec2& rhs) const;
    };

    vec2 operator+(const vec2& lhs, const vec2& rhs);
    vec2 operator-(const vec2& lhs, const vec2& rhs);

    vec2 operator*(const vec2& v, float scalar);
    vec2 operator*(float scalar, const vec2& v);
    vec2 operator/(const vec2& v, float scalar);
    vec2 operator/(float scalar, const vec2& v);

    bool operator==(const vec2& lhs, const vec2& rhs);
    bool operator!=(const vec2& lhs, const vec2& rhs);
}