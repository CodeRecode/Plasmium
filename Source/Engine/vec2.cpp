#include "vec2.h"
#include "math.h"

namespace Plasmium
{
    vec2& vec2::operator=(const vec2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    vec2& vec2::operator+=(const vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    vec2& vec2::operator-=(const vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    vec2& vec2::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    vec2& vec2::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float& vec2::operator[](uint32 index)
    {
        return (&x)[index];
    }

    vec2 vec2::Normalize() const
    {
        return (*this / Length());
    }

    float vec2::Length() const
    {
        return sqrtf(LengthSquared());
    }

    float vec2::LengthSquared() const
    {
        return (x * x) + (y * y);
    }

    float vec2::Dot(const vec2& rhs) const
    {
        return (x * rhs.x) + (y * rhs.y);
    }

    float vec2::DistanceSquared(const vec2& rhs) const {
        return (x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y);
    }

    vec2 operator+(const vec2& lhs, const vec2& rhs)
    {
        return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    vec2 operator-(const vec2& lhs, const vec2& rhs)
    {
        return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    vec2 operator*(const vec2& v, float scalar)
    {
        return vec2(v.x * scalar, v.y * scalar);
    }

    vec2 operator*(float scalar, const vec2& v)
    {
        return vec2(v.x * scalar, v.y * scalar);
    }

    vec2 operator/(const vec2& v, float scalar)
    {
        return vec2(v.x / scalar, v.y / scalar);
    }

    vec2 operator/(float scalar, const vec2& v)
    {
        return vec2(v.x / scalar, v.y / scalar);
    }

    bool operator==(const vec2& lhs, const vec2& rhs)
    {
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }

    bool operator!=(const vec2& lhs, const vec2& rhs)
    {
        return (lhs.x != rhs.x || lhs.y != rhs.y);
    }
}