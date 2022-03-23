#pragma once
#include "Component.h"
#include "vec3.h"

namespace Plasmium {
    class TransformComponent : public Component<TransformComponent>
    {
    private:
        vec3 logicalPosition;
        vec3 position;
        vec3 rotation;
        vec3 scale;

        mat4 worldMatrix;

    public:
        TransformComponent(EntityId entityId, vec3 logicalPosition, vec3 position, vec3 rotation, vec3 scale) :
            Component(entityId),
            logicalPosition(logicalPosition),
            position(position), 
            rotation(rotation),
            scale(scale)
        {
            UpdateWorldMatrix();
        }

        const vec3& GetLogicalPosition() const { return logicalPosition; }
        const vec3& GetPosition() const { return position; }
        const vec3& GetRotation() const { return rotation; }
        const vec3& GetScale() const { return scale; }
        mat4& GetWorldMatrix() { return worldMatrix; }

        void SetLogicalPosition(const vec3& newPosition) { logicalPosition = newPosition; }
        void SetPosition(const vec3& newPosition)
        {
            position = newPosition;
            UpdateWorldMatrix();
        }
        void SetRotation(const vec3& newRotation)
        {
            rotation = newRotation;
            UpdateWorldMatrix();
        }
        void SetScale(const vec3& newScale)
        {
            scale = newScale;
            UpdateWorldMatrix();
        }

        void UpdateWorldMatrix()
        {
            worldMatrix = mat4(1.0f);
            worldMatrix = worldMatrix.Translate(GetPosition());
            worldMatrix = worldMatrix.Rotate(GetRotation());
            worldMatrix = worldMatrix.Scale(GetScale());
        }

        // Logical is +x, +z, world is +x, -z and twice the scale
        static vec3 LogicalPointToWorld(const vec3& logical) {
            vec3 result = logical * 2.0f;
            result.z = -result.z;
            return result;
        }
    };
}