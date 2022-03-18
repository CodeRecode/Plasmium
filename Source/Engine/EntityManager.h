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
#include "AnimationManager.h"

namespace Plasmium {
    class EntityManager : public CoreSystem, public ComponentManager {
    private:
        struct TransformAnimation {
            milliseconds endTime;
            EntityId id;
            vec3 positionDiff;
            vec3 rotationDiff;
            vec3 finalPostion;
            vec3 finalRotation;
        };

        EntityId nextEntityId;
        Handler<EntityId, Entity> entities;
        Handler<EntityId, TransformComponent> transforms;

        HashTable<uint32, ComponentManager*> componentManagers;
        AnimationManager animationManager;
        TransformComponent* GetTransformInternal(EntityId id) { return transforms.GetObjectPtr(id); }

    public:
        Entity* CreateEntity();
        Entity* GetEntity(EntityId id);
        void DeleteEntity(EntityId id);
        void DeleteAllEntities();

        void RegisterComponentManager(ComponentType type, ComponentManager* manager);

        const TransformComponent* GetTransform(EntityId id) { return transforms.GetObjectPtr(id); }

        void Update(milliseconds deltaTime) override;
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

        void CreateComponent(const ComponentCreationArgs& creationArgs,
            const vec3& logicalPosition,
            const vec3& position,
            const vec3& rotation,
            const vec3& scale) override;

        void DeleteComponent(EntityId id, ComponentType type) override;
    };
}