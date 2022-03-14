#include "vec3.h"
#include "math.h"

namespace Plasmium
{
    vec3& vec3::operator=(const vec3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    vec3& vec3::operator+=(const vec3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    vec3& vec3::operator-=(const vec3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    vec3& vec3::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    vec3& vec3::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    float& vec3::operator[](uint32 index)
    {
        return (&x)[index];
    }

    vec3 vec3::Normalize() const
    {
        return (*this / Length());
    }

    float vec3::Length() const
    {
        return sqrtf(LengthSquared());
    }

    float vec3::LengthSquared() const
    {
        return (x * x) + (y * y) + (z * z);
    }

    float vec3::Dot(const vec3& rhs) const
    {
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
    }

    vec3 vec3::Cross(const vec3& rhs) const {
        return vec3((y * rhs.z) - (z * rhs.y), (z * rhs.x) - (x * rhs.z), (x * rhs.y) - (y * rhs.x));
    }

    vec3 operator+(const vec3& lhs, const vec3& rhs)
    {
        return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    vec3 operator-(const vec3& lhs, const vec3& rhs)
    {
        return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    vec3 operator*(const vec3& v, float scalar)
    {
        return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    vec3 operator*(float scalar, const vec3& v)
    {
        return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    vec3 operator/(const vec3& v, float scalar)
    {
        return vec3(v.x / scalar, v.y / scalar, v.z / scalar);
    }

    vec3 operator/(float scalar, const vec3& v)
    {
        return vec3(v.x / scalar, v.y / scalar, v.z / scalar);
    }

    bool operator==(const vec3& lhs, const vec3& rhs)
    {
        return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
    }

    bool operator!=(const vec3& lhs, const vec3& rhs)
    {
        return (lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z);
    }
}