#include "mat4.h"
#include "Plasmath.h"

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


    mat4 mat4::Transpose() const {
        mat4 result;
        for (uint32 i = 0; i < 4; ++i) {
            for (uint32 j = 0; j < 4; ++j) {
                result[i][j] = data[j][i];
            }
        }
        return result;
    }

    mat4 mat4::Scale(vec3 scale) const {
        mat4 result(*this);
        mat4 scaleM(1.0f);
        scaleM[0][0] *= scale.x;
        scaleM[1][1] *= scale.y;
        scaleM[2][2] *= scale.z;
        return scaleM * result;
    }

    mat4 mat4::Rotate(vec3 eulerAngles) const
    {
        mat4 result(*this);
        mat4 rotation;

        radians xRadians = DegreesToRadians(eulerAngles.x);
        radians yRadians = DegreesToRadians(eulerAngles.y);
        radians zRadians = DegreesToRadians(eulerAngles.z);

        float sinX = (float)sin(xRadians);
        float cosX = (float)cos(xRadians);
        float sinY = (float)sin(yRadians);
        float cosY = (float)cos(yRadians);
        float sinZ = (float)sin(zRadians);
        float cosZ = (float)cos(zRadians);

        // https://en.wikipedia.org/wiki/Rotation_matrix
        rotation[0][0] = cosZ * cosY;
        rotation[0][1] = (cosZ * sinY * sinX) - (sinZ * cosX);
        rotation[0][2] = (cosZ * sinY * cosX) + (sinZ * sinX);

        rotation[1][0] = sinZ * cosY;
        rotation[1][1] = (sinZ * sinY * sinX) + (cosZ * cosX);
        rotation[1][2] = (sinZ * sinY * cosX) - (cosZ * sinX);

        rotation[2][0] = -sinY;
        rotation[2][1] = cosY * sinX;
        rotation[2][2] = cosY * cosX;

        rotation[3][3] = 1.0f;

        return rotation * result;
    }

    mat4 mat4::Translate(vec3 translation) const {
        mat4 result(*this);
        mat4 translate(1.0f);
        translate[3][0] += translation.x;
        translate[3][1] += translation.y;
        translate[3][2] += translation.z;
        return translate * result;
    }

    mat4 mat4::ProjectionMatrix(radians fieldOfView, float aspectRatio, float zNear, float zFar)
    {
        mat4 result;
        float yScale = 1.0f / (float)tan(fieldOfView / 2.0f);
        float xScale = yScale / aspectRatio;
        float zDist = (zFar - zNear);

        result[0][0] = xScale;
        result[1][1] = yScale;
        result[2][2] = zFar / zDist;
        result[2][3] = 1;
        result[3][2] = (-zNear * zFar) / zDist;

        return result;
    }

    mat4 mat4::OrthogonalMatrix(float height, float width, float zNear, float zFar)
    {
        mat4 result;

        result[0][0] = 2.0f / width;
        result[1][1] = 2.0f / height;
        result[2][2] = 1.0f / (zFar - zNear);
        result[3][2] = zNear / (zNear - zFar);
        result[3][3] = 1.0f;

        return result;
    }

    mat4 operator*(const mat4& mat, float scalar)
    {
        mat4 result = mat;
        result *= scalar;
        return result;
    }

    vec3 operator*(const mat4& mat, const vec3& v)
    {
        vec3 row0(mat[0].x, mat[0].y, mat[0].z);
        vec3 row1(mat[1].x, mat[1].y, mat[1].z);
        vec3 row2(mat[2].x, mat[2].y, mat[2].z);

        return vec3(v.Dot(row0), v.Dot(row1), v.Dot(v));
    }

    vec4 operator*(const mat4& mat, const vec4& v)
    {
        return vec4(v.Dot(mat[0]), v.Dot(mat[1]), v.Dot(mat[2]), v.Dot(mat[3]));
    }

    mat4 operator*(const mat4& lhs, const mat4& rhs)
    {
        mat4 result;
        for (uint32 i = 0; i < 4; ++i) {
            for (uint32 j = 0; j < 4; ++j) {
                result[i][j] = lhs[i].Dot(rhs.GetCol(j));
            }
        }
        return result;
    }
}