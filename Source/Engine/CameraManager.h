#pragma once
#include "CameraComponent.h"
#include "ComponentManager.h"
#include "CoreSystem.h"
#include "Handler.h"

namespace Plasmium {
    class CameraManager : public CoreSystem, public ComponentManager {
    private:
        CameraComponent* currentCamera;
        Handler<EntityId, CameraComponent> cameraComponents;

    public:
        void Initialize() override;
        void ProcessEvent(const GenericEvent& event) override;
        void Update(milliseconds deltaTime) override;

        const Camera* GetCamera() const { return currentCamera; }
        void CreateComponent(const ComponentCreationArgs& creationArgs,
            const vec3& postionOffset,
            const vec3& rotation) override;
        void DeleteComponent(EntityId id, ComponentType type) override;
    };
}