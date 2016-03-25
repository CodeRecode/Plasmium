#pragma once

namespace Plasmium
{
    class vec3
    {
    public:
        float x, y, z;

        vec3() : x(0), y(0), z(0) { };
        vec3(float val) : x(val), y(val), z(val) { };
        vec3(float x, float y, float z) : x(x), y(y), z(z) { };

        vec3 operator=(const vec3& rhs);
        vec3 operator+=(const vec3& rhs);
        vec3 operator-=(const vec3& rhs);

        vec3 operator*=(float scalar);
        vec3 operator/=(float scalar);
    };

    vec3 operator+(const vec3& lhs, const vec3& rhs);
    vec3 operator-(const vec3& lhs, const vec3& rhs);

    vec3 operator*(const vec3& v, float scalar);
    vec3 operator*(float scalar, const vec3& v);
    vec3 operator/(const vec3& v, float scalar);
    vec3 operator/(float scalar, const vec3& v);

    vec3 operator==(const vec3& lhs, const vec3& rhs);
    vec3 operator!=(const vec3& lhs, const vec3& rhs);

    vec3 normalize(const vec3& v);
    float length(const vec3& v);
    float lengthSquared(const vec3& v);
    float dot(const vec3& lhs, const vec3& rhs);
}