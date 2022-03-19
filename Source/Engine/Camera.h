#pragma once

#include "mat4.h"
#include "vec2.h"
#include "vec3.h"

namespace Plasmium
{
    class Camera {
    protected:
        const vec3 Up = vec3(0.0, 1.0, 0.0);

        vec3 GetCameraDirection() const;
    public:
        virtual vec3 GetPosition() const = 0;
        virtual vec3 GetRotation() const = 0;

        virtual void MovePositionRelative(vec2 move) {}
        virtual void MoveRotation(vec3 rotation) {}
        virtual void SetPosition(vec3 position) {};
        virtual void SetRotation(vec3 rotation) {};
        virtual void Zoom(float move) {};

        vec3 GetPositionRelative(const vec2& move) const;
        mat4 GetViewMatrix() const;
    };
}