#pragma once
#include "Types.h"
#include "vec4.h"

namespace Plasmium
{
    class mat4
    {
    private:
        vec4 data[4];

    public:
        mat4() { }; // Zero matrix
        mat4(float val);
        mat4(const mat4& mat);

        mat4& operator=(const mat4& rhs);

        mat4& operator*=(float scalar);
        mat4& operator*=(const mat4& rhs);

        vec4& operator[](uint32 index);
        const vec4& operator[](uint32 index) const;
    };

    mat4 operator*(const mat4& mat, float scalar);
    vec4 operator*(const mat4& mat, const vec4& v);
    mat4 operator*(const mat4& lhs, const mat4& rhs);
}