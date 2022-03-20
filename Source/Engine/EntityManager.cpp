#include "EntityManager.h"

#include "AllComponents.h"
#include "Core.h"
#include "Plasmath.h"


namespace Plasmium {
    EntityId EntityManager::CreateEntity()
    {
        auto* entity = entities.EmplaceObject(nextEntityId, Entity(nextEntityId));
        ++nextEntityId;

        return entity->GetId();
    }

    void EntityManager::DeleteEntity(EntityId id)
    {
        if (!entities.Contains(id)) {
            return;
        }

        for (auto& handlerPair : componentHandlers) {
            handlerPair.value->DeleteObject(id);
        }
        entities.DeleteObject(id);
    }

    void EntityManager::DeleteAllEntities()
    {
        while (entities.GetObjectsReference().Size() > 0) {
            auto& entity = entities.GetObjectsReference()[entities.GetObjectsReference().Size() - 1];
            DeleteEntity(entity.GetId());
        }
    }

    void EntityManager::Initialize()
    {
        componentHandlers[CameraComponent::GetType()] = std::make_shared<Handler<EntityId, CameraComponent>>();
        componentHandlers[CombatComponent::GetType()] = std::make_shared<Handler<EntityId, CombatComponent>>();
        componentHandlers[ModelComponent::GetType()] = std::make_shared<Handler<EntityId, ModelComponent>>();
        componentHandlers[MonsterControllerComponent::GetType()] = std::make_shared<Handler<EntityId, MonsterControllerComponent>>();
        componentHandlers[NameComponent::GetType()] = std::make_shared<Handler<EntityId, NameComponent>>();
        componentHandlers[PlayerControllerComponent::GetType()] = std::make_shared<Handler<EntityId, PlayerControllerComponent>>();
        componentHandlers[TransformComponent::GetType()] = std::make_shared<Handler<EntityId, TransformComponent>>();
    }

    void EntityManager::ProcessEvent(const GenericEvent& event)
    {
        if ((EventType)event.index() == EventType::ChangeTransform) {
            auto& changeTransform = std::get<ChangeTransformEvent>(event);
            auto& transform = *GetComponent<TransformComponent>(changeTransform.entityId);
            if (changeTransform.changeValues & ChangeTransformLogicalPosition)
            {
                transform.SetLogicalPosition(changeTransform.logicalPosition);
            }
            if (changeTransform.changeValues & ChangeTransformPosition)
            {
                transform.SetPosition(changeTransform.position);
            }
            if (changeTransform.changeValues & ChangeTransformRotation)
            {
                transform.SetRotation(changeTransform.rotation);
            }
            if (changeTransform.changeValues & ChangeTransformScale)
            {
                transform.SetScale(changeTransform.scale);
            }
        }
        if ((EventType)event.index() == EventType::DestroyComponent) {
            auto& destroyComponent = std::get<DestroyComponentEvent>(event);
            componentHandlers[destroyComponent.type]->DeleteObject(destroyComponent.entityId);
        }
        if ((EventType)event.index() == EventType::DestroyEntity) {
            auto& destroyEntity = std::get<DestroyEntityEvent>(event);

            DeleteEntity(destroyEntity.entityId);
        }
    }
}