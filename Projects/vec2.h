#pragma once

namespace Plasmium
{
    class vec2
    {
    public:
        float x, y;

        vec2() : x(0), y(0) { };
        vec2(float val) : x(val), y(val) { };
        vec2(float x, float y) : x(x), y(y) { };

        vec2 operator=(const vec2& rhs);
        vec2 operator+=(const vec2& rhs);
        vec2 operator-=(const vec2& rhs);

        vec2 operator*=(float scalar);
        vec2 operator/=(float scalar);
    };

    vec2 operator+(const vec2& lhs, const vec2& rhs);
    vec2 operator-(const vec2& lhs, const vec2& rhs);

    vec2 operator*(const vec2& v, float scalar);
    vec2 operator*(float scalar, const vec2& v);
    vec2 operator/(const vec2& v, float scalar);
    vec2 operator/(float scalar, const vec2& v);

    vec2 operator==(const vec2& lhs, const vec2& rhs);
    vec2 operator!=(const vec2& lhs, const vec2& rhs);

    vec2 normalize(const vec2& v);
    float length(const vec2& v);
    float lengthSquared(const vec2& v);
    float dot(const vec2& lhs, const vec2& rhs);
}