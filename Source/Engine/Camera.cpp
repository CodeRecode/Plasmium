#include "Camera.h"
#include <math.h>
#include "Plasmath.h"

namespace Plasmium
{

    vec3 Camera::GetCameraDirection() const {
        vec3 rotation = GetRotation();
        radians xRadians = DegreesToRadians(rotation.x);
        radians yRadians = DegreesToRadians(rotation.y);
        float sinX = (float)sin(xRadians);
        float cosX = (float)cos(xRadians);
        float sinY = (float)sin(yRadians);
        float cosY = (float)cos(yRadians);
        return vec3(cosX * cosY, sinY, sinX * cosY).Normalize();
    }

    vec3 Camera::GetPositionRelative(const vec2& move) const
    {
        vec3 cameraDirection = GetCameraDirection();
        // Find camera direction projected onto the y plane
        vec3 yPlane = vec3(cameraDirection.x, 0.0f, cameraDirection.z).Normalize();

        // x is side to side, y is forwards/backwards
        vec3 xDir = Up.Cross(yPlane).Normalize() * move.x;
        vec3 zDir = yPlane * move.y;

        return xDir + zDir;
    }

    mat4 Camera::GetViewMatrix() const
    {
        mat4 result;
        vec3 position = GetPosition();
        vec3 lookAt = position + GetCameraDirection();

        // https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        vec3 zAxis = (lookAt - position).Normalize();
        vec3 xAxis = Up.Cross(zAxis).Normalize();
        vec3 yAxis = zAxis.Cross(xAxis);

        result[0] = vec4(xAxis.x, yAxis.x, zAxis.x, 0.0f);
        result[1] = vec4(xAxis.y, yAxis.y, zAxis.y, 0.0f);
        result[2] = vec4(xAxis.z, yAxis.z, zAxis.z, 0.0f);
        result[3] = vec4(-xAxis.Dot(position),
            -yAxis.Dot(position),
            -zAxis.Dot(position),
            1.0f);

        return result;
    }
}