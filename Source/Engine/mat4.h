#pragma once
#include "Types.h"
#include "vec3.h"
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

        vec4 GetCol(uint32 col) const { return vec4(data[0][col], data[1][col], data[2][col], data[3][col]); }

        mat4& operator=(const mat4& rhs);

        mat4& operator*=(float scalar);
        mat4& operator*=(const mat4& rhs);

        vec4& operator[](uint32 index);
        const vec4& operator[](uint32 index) const;

        mat4 Transpose() const;
        mat4 Scale(vec3 scale) const;
        mat4 Rotate(vec3 eulerAngles) const;
        mat4 Translate(vec3 translation) const;

        static mat4 ProjectionMatrix(radians fieldOfView, float aspectRatio, float zNear, float zFar);
        static mat4 OrthogonalMatrix(float height, float width, float zNear, float zFar);
    };

    mat4 operator*(const mat4& mat, float scalar);
    vec3 operator*(const mat4& mat, const vec3& v);
    vec4 operator*(const mat4& mat, const vec4& v);
    mat4 operator*(const mat4& lhs, const mat4& rhs);
}