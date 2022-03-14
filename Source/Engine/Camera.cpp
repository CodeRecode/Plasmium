#include "Camera.h"
#include <math.h>
#include "Plasmath.h"

namespace Plasmium
{

    vec3 Camera::GetCameraDirection() {
        radians xRadians = DegreesToRadians(rotation.x);
        radians yRadians = DegreesToRadians(rotation.y);
        float sinX = (float)sin(xRadians);
        float cosX = (float)cos(xRadians);
        float sinY = (float)sin(yRadians);
        float cosY = (float)cos(yRadians);
        return vec3(cosX * cosY, sinY, sinX * cosY).Normalize();
    }

    void Camera::UpdateViewMatrix()
    {
        vec3 lookAt = position + GetCameraDirection();

        // https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        vec3 zAxis = (lookAt - position).Normalize();
        vec3 xAxis = up.Cross(zAxis).Normalize();
        vec3 yAxis = zAxis.Cross(xAxis);

        viewMatrix[0] = vec4(xAxis.x, yAxis.x, zAxis.x, 0.0f);
        viewMatrix[1] = vec4(xAxis.y, yAxis.y, zAxis.y, 0.0f);
        viewMatrix[2] = vec4(xAxis.z, yAxis.z, zAxis.z, 0.0f);
        viewMatrix[3] = vec4(-xAxis.Dot(position), 
            -yAxis.Dot(position),
            -zAxis.Dot(position),
            1.0f);
    }

    void Camera::SetPosition(vec3 newPosition)
    {
        position = newPosition;
        UpdateViewMatrix();
    }

    void Camera::MovePosition(vec3 move)
    {
        position += move;
        UpdateViewMatrix();
    }

    void Camera::MovePositionRelative(vec2 move)
    {
        vec3 cameraDirection = GetCameraDirection();
        // Find camera direction projected onto the y plane
        vec3 yPlane = vec3(cameraDirection.x, 0.0f, cameraDirection.z).Normalize();

        // x is side to side, y is forwards/backwards
        vec3 xDir = up.Cross(yPlane).Normalize() * move.x;
        vec3 zDir = yPlane * move.y;

        position += xDir;
        position += zDir;
        UpdateViewMatrix();
    }

    void Camera::Zoom(float move)
    {
        position += GetCameraDirection() * move;
        UpdateViewMatrix();
    }

    void Camera::SetRotation(vec3 newRotation)
    {
        rotation = newRotation;
        UpdateViewMatrix();
    }

    void Camera::MoveRotation(vec3 move)
    {
        rotation += move;
        UpdateViewMatrix();
    }

    const mat4& Camera::GetViewMatrix() const
    {
        return viewMatrix;
    }
}