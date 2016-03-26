#include "vec2.h"
#include "math.h"

namespace Plasmium
{
    inline vec2& vec2::operator=(const vec2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    inline vec2& vec2::operator+=(const vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    inline vec2& vec2::operator-=(const vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    inline vec2& vec2::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    inline vec2& vec2::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    inline float& vec2::operator[](uint32 index)
    {
        return (&x)[index];
    }

    inline vec2 operator+(const vec2& lhs, const vec2& rhs)
    {
        return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    inline vec2 operator-(const vec2& lhs, const vec2& rhs)
    {
        return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    inline vec2 operator*(const vec2& v, float scalar)
    {
        return vec2(v.x * scalar, v.y * scalar);
    }

    inline vec2 operator*(float scalar, const vec2& v)
    {
        return vec2(v.x * scalar, v.y * scalar);
    }

    inline vec2 operator/(const vec2& v, float scalar)
    {
        return vec2(v.x / scalar, v.y / scalar);
    }

    inline vec2 operator/(float scalar, const vec2& v)
    {
        return vec2(v.x / scalar, v.y / scalar);
    }

    inline vec2 operator==(const vec2& lhs, const vec2& rhs)
    {
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }

    inline vec2 operator!=(const vec2& lhs, const vec2& rhs)
    {
        return (lhs.x != rhs.x || lhs.y != rhs.y);
    }

    inline vec2 normalize(const vec2& v)
    {
        return (v / length(v));
    }

    inline float length(const vec2& v)
    {
        return sqrtf(lengthSquared(v));
    }

    inline float lengthSquared(const vec2& v)
    {
        return (v.x * v.x + v.y * v.y);
    }

    inline float dot(const vec2& lhs, const vec2& rhs)
    {
        return (lhs.x * rhs.x + lhs.y * rhs.y);
    }
}