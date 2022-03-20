#pragma once
#include "Types.h"
#include "Component.h"
#include "Entity.h"
#include "Handler.h"
#include "Event.h"
#include "CoreSystem.h"

#include <assert.h>

namespace Plasmium {

    class EntityManager : public CoreSystem {
    private:
        EntityId nextEntityId;
        Handler<EntityId, Entity> entities;
        HashTable<ComponentType, std::shared_ptr<IHandler<EntityId>>> componentHandlers;

    public:
        EntityId CreateEntity();
        void DeleteEntity(EntityId id);
        void DeleteAllEntities();

        void Initialize() override;
        void ProcessEvent(const GenericEvent& event) override;

        template <typename T, typename... Args>
        void AddComponent(EntityId id, Args&&... args)
        {
            auto handler = std::static_pointer_cast<Handler<EntityId, T>>(componentHandlers[T::GetType()]);
            handler->EmplaceObject(id, T(id, std::forward<Args>(args)...));
            entities.GetObjectPtr(id)->AddComponentType(T::GetType());
        }

        template <typename T>
        T* GetComponent(EntityId id)
        {
            auto handlerPtr = std::static_pointer_cast<Handler<EntityId, T>>(componentHandlers[T::GetType()]);
            return handlerPtr->GetObjectPtr(id);
        }

        template <typename T>
        const Array<T>& GetComponentArray()
        {
            auto handler = std::static_pointer_cast<Handler<EntityId, T>>(componentHandlers[T::GetType()]);
            return handler->GetObjectsReference();
        }

        template <typename T>
        void DeleteComponent(EntityId id)
        {
            auto handler = std::static_pointer_cast<Handler<EntityId, T>>(componentHandlers[T::GetType()]);
            return handler->DeleteObject(id);
        }
    };
}