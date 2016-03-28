#include "mat4.h"

namespace Plasmium
{
    mat4::mat4(float val)
    {
        data[0][0] = data[1][1] = data[2][2] = data[3][3] = val;
    }

    mat4::mat4(const mat4& mat)
    {
        *this = mat;
    }

    mat4& mat4::operator=(const mat4& rhs)
    {
        data[0] = rhs[0];
        data[1] = rhs[1];
        data[2] = rhs[2];
        data[3] = rhs[3];
        return *this;
    }

    mat4& mat4::operator*=(float scalar)
    {
        data[0] *= scalar;
        data[1] *= scalar;
        data[2] *= scalar;
        data[3] *= scalar;
        return *this;
    }

    mat4& mat4::operator*=(const mat4& rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    vec4& mat4::operator[](uint32 index)
    {
        return data[index];
    }

    const vec4& mat4::operator[](uint32 index) const
    {
        return data[index];
    }

    mat4 operator*(const mat4& mat, float scalar)
    {
        mat4 result = mat;
        result *= scalar;
        return result;
    }

    vec4 operator*(const mat4& mat, const vec4& v)
    {
        return vec4(dot(mat[0], v), dot(mat[1], v), dot(mat[2], v), dot(mat[3], v));
    }

    mat4 operator*(const mat4& lhs, const mat4& rhs)
    {
        mat4 result = lhs;
        return result;
    }
}