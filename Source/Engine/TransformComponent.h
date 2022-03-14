#pragma once
#include "Component.h"
#include "vec3.h"

namespace Plasmium {
    class TransformComponent : public Component
    {
    private:
        vec3 position;
        vec3 rotation;
        vec3 scale;
        bool moving;

    public:
        TransformComponent(const ComponentCreationArgs& args, vec3 position, vec3 rotation, vec3 scale) :
            Component(args),
            position(position), 
            rotation(rotation),
            scale(scale),
            moving(false)
        { }
        const vec3& GetLogicalPosition() { return WorldPointToLogical(position); }
        const vec3& GetPosition() { return position; }
        const vec3& GetRotation() { return rotation; }
        const vec3& GetScale() { return scale; }
        const bool GetMoving() { return moving; }

        void SetPosition(const vec3& newPosition) { position = newPosition; }
        void SetRotation(const vec3& newRotation) { rotation = newRotation; }
        void SetScale(const vec3& newScale) { scale = newScale; }
        void SetMoving(bool newMoving) { moving = newMoving; }

        void AddPosition(const vec3& deltaPosition) { position += deltaPosition; }
        void AddRotation(const vec3& deltaRotation) { rotation += deltaRotation; }
        void AddScale(const vec3& deltaScale) { scale += deltaScale; }


        // Logical is +x, +z, world is +x, -z and twice the scale
        static vec3 LogicalPointToWorld(const vec3& logical) {
            vec3 result = logical * 2.0f;
            result.z = -result.z;
            return result;
        }
        static vec3 WorldPointToLogical(const vec3& world) {
            vec3 result = world / 2.0f;
            result.z = -result.z;
            return result;
        }
    };
}