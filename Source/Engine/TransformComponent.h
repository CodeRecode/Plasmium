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

    public:
        TransformComponent(EntityId entityId, vec3 logicalPosition, vec3 position, vec3 rotation, vec3 scale) :
            Component(entityId),
            logicalPosition(logicalPosition),
            position(position), 
            rotation(rotation),
            scale(scale)
        { }
        const vec3& GetLogicalPosition() const { return logicalPosition; }
        const vec3& GetPosition() const { return position; }
        const vec3& GetRotation() const { return rotation; }
        const vec3& GetScale() const { return scale; }

        void SetLogicalPosition(const vec3& newPosition) { logicalPosition = newPosition; }
        void SetPosition(const vec3& newPosition) { position = newPosition; }
        void SetRotation(const vec3& newRotation) { rotation = newRotation; }
        void SetScale(const vec3& newScale) { scale = newScale; }

        void AddPosition(const vec3& deltaPosition) { position += deltaPosition; }
        void AddRotation(const vec3& deltaRotation) { rotation += deltaRotation; }
        void AddScale(const vec3& deltaScale) { scale += deltaScale; }


        // Logical is +x, +z, world is +x, -z and twice the scale
        static vec3 LogicalPointToWorld(const vec3& logical) {
            vec3 result = logical * 2.0f;
            result.z = -result.z;
            return result;
        }
    };
    ComponentType TransformComponent::type = ComponentType::Transform;
}