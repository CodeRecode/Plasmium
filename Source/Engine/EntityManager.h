#pragma once
#include "Types.h"
#include "Component.h"
#include "Entity.h"
#include "Handler.h"
#include "TransformComponent.h"
#include "ComponentManager.h"
#include <assert.h>
#include "Event.h"
#include "CoreSystem.h"
#include "PlayerControllerComponent.h"

namespace Plasmium {
    class EntityManager : public CoreSystem, public ComponentManager {
    private:
        struct TransformAnimation {
            uint64 endFrame;
            EntityId id;
            vec3 positionStep;
            vec3 rotationStep;
            vec3 finalPostion;
            vec3 finalRotation;
        };

        EntityId nextEntityId;
        Handler<EntityId, Entity> entities;
        Handler<EntityId, TransformComponent> transforms;
        PlayerControllerComponent* playerControllerComponent;

        HashTable<uint32, ComponentManager*> componentManagers;
        Array <TransformAnimation> animations;

    public:
        Entity* CreateEntity();
        Entity* GetEntity(EntityId id);
        void DeleteEntity(EntityId id);

        void RegisterComponentManager(ComponentType type, ComponentManager* manager);

        TransformComponent* GetTransform(EntityId id) { return transforms.GetObjectPtr(id); }

        void Update() override;
        void ProcessEvent(const GenericEvent& event) override;

        template<typename... Args>
        void AddComponent(Entity* entity, ComponentType type, Args&&... args)
        {
            assert(componentManagers.Contains((uint32)type) && "Component manager not registered");
            auto* manager = componentManagers[(uint32)type];
            manager->CreateComponentBase<Args...>(
                ComponentCreationArgs(type, entity->GetId()),
                std::forward<Args>(args)...);
            entity->AddComponentType(type);
        }

        void CreateComponent(const ComponentCreationArgs& creationArgs) override;
        void CreateComponent(const ComponentCreationArgs& creationArgs,
            const vec3& position,
            const vec3& rotation,
            const vec3& scale) override;

        void DeleteComponent(EntityId id, ComponentType type) override;
    };
}