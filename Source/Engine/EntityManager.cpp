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
        milliseconds currentTime = Core::GetInstance().GetFrameStartTime();
        for (uint32 index = 0; index < animations.Size(); ++index) {
            auto& animation = animations[index];
            auto* transform = GetTransform(animation.id);
            if (transform == nullptr) {
                animations.Delete(index);
                --index;
                continue;
            }

            if (currentTime >= animation.endTime) {
                transform->SetPosition(animation.finalPostion);
                transform->SetRotation(animation.finalRotation);
                transform->SetMoving(false);
                animations.Delete(index);
                --index;
                continue;
            }
            transform->AddPosition(animation.positionDiff * deltaTime);
            transform->AddRotation(animation.rotationDiff * deltaTime);
        }
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
            if (transform.GetMoving()) {
                return;
            }

            milliseconds startTime = Core::GetInstance().GetFrameStartTime();
            const milliseconds AnimationTime = 150;

            vec3 finalPostion = moveEvent.position;
            vec3 finalRotation = moveEvent.rotation;
            vec3 positionDiff;
            vec3 rotationDiff;
            if (moveEvent.positionRelative) {
                finalPostion += transform.GetPosition();
                positionDiff = moveEvent.position;
            }
            else {
                positionDiff = (moveEvent.position - transform.GetPosition());
            }

            if (moveEvent.rotationRelative) {
                finalRotation += transform.GetRotation();
                rotationDiff = moveEvent.rotation;
            }
            else {
                float angle = FindTurningAngle(transform.GetRotation().y, 
                    moveEvent.rotation.y);
                rotationDiff.y = angle;
            }

            TransformAnimation animation;
            animation.id = moveEvent.entity;
            animation.endTime = startTime + AnimationTime;
            animation.finalPostion = finalPostion;
            animation.finalRotation = finalRotation;
            animation.positionDiff = positionDiff / AnimationTime;
            animation.rotationDiff = rotationDiff / AnimationTime;
            animations.Push(animation);
            transform.SetMoving(true);
        }
    }
    void EntityManager::CreateComponent(const ComponentCreationArgs& creationArgs)
    {
        assert(creationArgs.type == ComponentType::PlayerController);
        playerControllerComponent = new PlayerControllerComponent(creationArgs);
    }

    void EntityManager::CreateComponent(const ComponentCreationArgs& creationArgs,
        const vec3& position,
        const vec3& rotation,
        const vec3& scale)
    {
        assert(creationArgs.type == ComponentType::Transform);
        transforms.EmplaceObject(creationArgs.parent, TransformComponent(creationArgs, position, rotation, scale));
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