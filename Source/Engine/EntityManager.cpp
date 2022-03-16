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
        auto& entity = *GetEntity(id);
        auto& componentTypes = entity.GetComponentTypes();
        for (auto componentType : componentTypes) {
            componentManagers[componentType]->DeleteComponent(id, (ComponentType)componentType);
        }
        entities.DeleteObject(id);
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
        if ((EventType)event.index() == EventType::Input) {
            auto& input = std::get<InputEvent>(event);
            if (playerControllerComponent != nullptr) {
                playerControllerComponent->ProcessInput(input);
            }
        }
        if ((EventType)event.index() == EventType::MoveEntity) {
            auto& moveEvent = std::get<MoveEntityEvent>(event);
            auto& transform = *GetTransform(moveEvent.entity);
            if (transform.GetAnimating()) {
                return;
            }

            vec3 logicalDestination = moveEvent.logicalPosition;
            vec3 finalPostion = moveEvent.position;
            vec3 finalRotation = moveEvent.rotation;

            if (moveEvent.positionType == MovementType::Relative) {
                finalPostion += transform.GetPosition();
                logicalDestination += transform.GetLogicalPosition();
            }
            if (moveEvent.rotationType == MovementType::Relative) {
                finalRotation += transform.GetRotation();
            }

            const auto& levelManager = Core::GetInstance().GetLevelManager();
            if (!levelManager.IsWalkable(logicalDestination)) {
                animationManager.CreateBumpAnimation(moveEvent.entity,
                    finalRotation);
                return;
            }

            transform.SetLogicalPosition(logicalDestination);

            animationManager.CreateWalkAnimation(moveEvent.entity, 
                finalPostion,
                finalRotation);
        }
    }
    void EntityManager::CreateComponent(const ComponentCreationArgs& creationArgs)
    {
        assert(creationArgs.type == ComponentType::PlayerController);
        playerControllerComponent = new PlayerControllerComponent(creationArgs);
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
        assert(type == ComponentType::Transform || type == ComponentType::PlayerController);
        if (type == ComponentType::Transform) {
            transforms.DeleteObject(id);
        }
        else if (type == ComponentType::PlayerController) {
            delete playerControllerComponent;
        }
    }
}