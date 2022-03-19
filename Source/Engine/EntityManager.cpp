#include "EntityManager.h"

#include "Plasmath.h"
#include "Core.h"


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
        auto& entity = *entities.GetObjectPtr(id);
        auto& componentTypes = entity.GetComponentTypes();
        entities.DeleteObject(id);
    }

    void EntityManager::DeleteAllEntities()
    {
        animationManager.StopAll();
        while (entities.GetObjectsReference().Size() > 0) {
            auto& entity = entities.GetObjectsReference()[entities.GetObjectsReference().Size() - 1];
            DeleteEntity(entity.GetId());
        }
    }

    void EntityManager::Initialize()
    {
        componentHandlers[CameraComponent::GetType()] = Handler<EntityId, CameraComponent>();
        componentHandlers[CombatComponent::GetType()] = Handler<EntityId, CombatComponent>();
        componentHandlers[ModelComponent::GetType()] = Handler<EntityId, ModelComponent>();
        componentHandlers[MonsterControllerComponent::GetType()] = Handler<EntityId, MonsterControllerComponent>();
        componentHandlers[NameComponent::GetType()] = Handler<EntityId, NameComponent>();
        componentHandlers[PlayerControllerComponent::GetType()] = Handler<EntityId, PlayerControllerComponent>();
        componentHandlers[TransformComponent::GetType()] = Handler<EntityId, TransformComponent>();
    }

    void EntityManager::Update(milliseconds deltaTime)
    {
        animationManager.Update(deltaTime);
    }

    void EntityManager::ProcessEvent(const GenericEvent& event)
    {
        if ((EventType)event.index() == EventType::AnimateEntity) {
            auto& animateEntity = std::get<AnimateEntityEvent>(event);
            animationManager.CreateAnimation(animateEntity.params);
        }
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
            componentHandlers[destroyComponent.type];
            // TODO fix this
        }
        if ((EventType)event.index() == EventType::DestroyEntity) {
            auto& destroyEntity = std::get<DestroyEntityEvent>(event);

            DeleteEntity(destroyEntity.entityId);
        }
    }
}