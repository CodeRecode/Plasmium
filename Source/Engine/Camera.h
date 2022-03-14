#pragma once

#include "mat4.h"
#include "vec2.h"
#include "vec3.h"

namespace Plasmium
{
    class Camera {
    private:
        const vec3 up = vec3(0.0, 1.0, 0.0);
        vec3 position;
        vec3 rotation; // Degrees: Pitch, Roll, Yaw
        mat4 viewMatrix;

        vec3 Camera::GetCameraDirection();
        void UpdateViewMatrix();
    public:
        vec3 GetPosition() const { return position; }

        void SetPosition(vec3 newPosition);
        void MovePosition(vec3 move);
        void MovePositionRelative(vec2 move);
        void Zoom(float move);
        void SetRotation(vec3 newRotation);
        void MoveRotation(vec3 move);
        const mat4& GetViewMatrix() const;
    };
}