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

    vec3 operator==(const vec3& lhs, const vec3& rhs)
    {
        return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
    }

    vec3 operator!=(const vec3& lhs, const vec3& rhs)
    {
        return (lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z);
    }

    vec3 normalize(const vec3& v)
    {
        return (v / length(v));
    }

    float length(const vec3& v)
    {
        return sqrtf(lengthSquared(v));
    }

    float lengthSquared(const vec3& v)
    {
        return (v.x * v.x + v.y * v.y + v.z * v.z);
    }

    float dot(const vec3& lhs, const vec3& rhs)
    {
        return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
    }
}