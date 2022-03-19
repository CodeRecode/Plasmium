#include "CameraComponent.h"

#include "Core.h"
#include "EntityManager.h"

namespace Plasmium {

    vec3 CameraComponent::GetPosition() const
    {
        return cachedPosition + positionOffset;
    }

    vec3 CameraComponent::GetRotation() const
    {
        return rotation;
    }

    void CameraComponent::Update(milliseconds deltaTime)
    {
        // Use a dead zone and lerping
        auto& transform = *Core::GetInstance().GetComponent<TransformComponent>(GetId());
        float distanceSq = (cachedPosition - transform.GetPosition()).LengthSquared();
        if (distanceSq > 8) {
            vec3 delta = transform.GetPosition() - cachedPosition;
            cachedPosition += delta * (float)deltaTime / 1000.f;
        }
    }

    void CameraComponent::MovePositionRelative(vec2 move)
    {
        positionOffset += GetPositionRelative(move);
    }

    void CameraComponent::MoveRotation(vec3 rotation)
    {
        vec3 yRotation = vec3(0.0f, -rotation.x, 0.0f);
        positionOffset = mat4(1.0f).Rotate(yRotation) * positionOffset;
        this->rotation += rotation;
    }

    void CameraComponent::SetPosition(vec3 position)
    {
        cachedPosition = position;
    }

    void CameraComponent::SetRotation(vec3 rotation)
    {
        this->rotation = rotation;
    }

    void CameraComponent::Zoom(float move)
    {
        positionOffset += GetCameraDirection() * move;
    }
}