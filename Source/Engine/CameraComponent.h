#pragma once
#include "Camera.h"
#include "Component.h"

namespace Plasmium
{
    class CameraComponent : public Camera, public Component {
    private:
        vec3 positionOffset;
        vec3 cachedPosition;
        vec3 rotation;
    public:
        CameraComponent(const ComponentCreationArgs& args, 
            const vec3& positionOffset, 
            const vec3& rotation) :
            Component(args),
            positionOffset(positionOffset),
            rotation(rotation)
        {}

        vec3 GetPosition() const override;
        vec3 GetRotation() const override;

        void Update(milliseconds deltaTime);

        void MovePositionRelative(vec2 move) override;
        void MoveRotation(vec3 rotation) override;
        void Zoom(float move) override;
    };
}