#include "EntityManager.h"

#include "Plasmath.h"
#include "Core.h"


namespace Plasmium {
    Entity* EntityManager::CreateEntity()
    {
        auto* entity = entities.EmplaceObject(nextEntityId, Entity(nextEntityId));
        ++nextEntityId;

        return entity;
    }

    Entity* EntityManager::GetEntity(EntityId id)
    {
        return entities.GetObjectPtr(id);
    }

    void EntityManager::DeleteEntity(EntityId id)
    {
        if (!entities.Contains(id)) {
            return;
        }
        auto& entity = *GetEntity(id);
        auto& componentTypes = entity.GetComponentTypes();

        // Allow components to remove state dependent on other components
        for (auto componentType : componentTypes) {
            componentManagers[componentType]->PreDeleteComponent(id, (ComponentType)componentType);
        }
        for (auto componentType : componentTypes) {
            componentManagers[componentType]->DeleteComponent(id, (ComponentType)componentType);
        }
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

    void EntityManager::RegisterComponentManager(ComponentType type, ComponentManager* manager)
    {
        componentManagers[(uint32)type] = manager;
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
            auto& transform = *GetTransformInternal(changeTransform.entityId);
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

            componentManagers[(uint32)destroyComponent.type]->DeleteComponent(
                destroyComponent.entityId, 
                destroyComponent.type);
        }
        if ((EventType)event.index() == EventType::DestroyEntity) {
            auto& destroyEntity = std::get<DestroyEntityEvent>(event);

            DeleteEntity(destroyEntity.entityId);
        }
    }

    void EntityManager::CreateComponent(const ComponentCreationArgs& creationArgs,
        const vec3& logicalPosition,
        const vec3& position,
        const vec3& rotation,
        const vec3& scale)
    {
        assert(creationArgs.type == ComponentType::Transform);
        transforms.EmplaceObject(creationArgs.parent, TransformComponent(
            creationArgs, 
            logicalPosition, 
            position, 
            rotation, 
            scale));
    }

    void EntityManager::DeleteComponent(EntityId id, ComponentType type)
    {
        assert(type == ComponentType::Transform);
        if (type == ComponentType::Transform) {
            transforms.DeleteObject(id);
        }
    }
}