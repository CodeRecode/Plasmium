#pragma once
#include "Types.h"
#include "Component.h"
#include "Entity.h"
#include "Handler.h"
#include "Event.h"
#include "CoreSystem.h"
#include "AnimationManager.h"

#include "CameraComponent.h"
#include "CombatComponent.h"
#include "ModelComponent.h"
#include "MonsterControllerComponent.h"
#include "NameComponent.h"
#include "PlayerControllerComponent.h"
#include "TransformComponent.h"

#include <assert.h>
#include <variant>

namespace Plasmium {
    typedef std::variant<Handler<EntityId, CameraComponent>,
        Handler<EntityId, CombatComponent>,
        Handler<EntityId, ModelComponent>,
        Handler<EntityId, MonsterControllerComponent>,
        Handler<EntityId, NameComponent>,
        Handler<EntityId, PlayerControllerComponent>,
        Handler<EntityId, TransformComponent>> ComponentHandler;

    class EntityManager : public CoreSystem {
    private:
        EntityId nextEntityId;
        Handler<EntityId, Entity> entities;
        HashTable<ComponentType, ComponentHandler> componentHandlers;

        AnimationManager animationManager;

    public:
        EntityId CreateEntity();
        void DeleteEntity(EntityId id);
        void DeleteAllEntities();

        void Initialize() override;
        void Update(milliseconds deltaTime) override;
        void ProcessEvent(const GenericEvent& event) override;

        template <typename T, typename... Args>
        void AddComponent(EntityId id, Args&&... args)
        {
            auto& handler = std::get<Handler<EntityId, T>>(componentHandlers[T::GetType()]);
            handler.EmplaceObject(id, T(id, std::forward<Args>(args)...));
            entities.GetObjectPtr(id)->AddComponentType(T::GetType());
        }

        template <typename T>
        T* GetComponent(EntityId id)
        {
            auto& handler = std::get<Handler<EntityId, T>>(componentHandlers[T::GetType()]);
            return static_cast<T*>(handler.GetObjectPtr(id));
        }

        template <typename T>
        const Array<T>& GetComponentArray()
        {
            auto& handler = std::get<Handler<EntityId, T>>(componentHandlers[T::GetType()]);
            return handler.GetObjectsReference();
        }

        template <typename T>
        void DeleteComponent(EntityId id)
        {
            auto& handler = std::get<Handler<EntityId, T>>(componentHandlers[T::GetType()]);
            return static_cast<T*>(handler.DeleteObject(id));
        }
    };
}