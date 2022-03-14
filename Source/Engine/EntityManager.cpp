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

    void EntityManager::Update()
    {
        uint64 frame = Core::GetInstance().GetFrame();
        for (uint32 index = 0; index < animations.Size(); ++index) {
            auto& animation = animations[index];
            auto* transform = GetTransform(animation.id);
            if (transform == nullptr) {
                animations.Delete(index);
                --index;
                continue;
            }

            if (frame >= animation.endFrame) {
                transform->SetPosition(animation.finalPostion);
                transform->SetRotation(animation.finalRotation);
                transform->SetMoving(false);
                animations.Delete(index);
                --index;
                continue;
            }
            transform->AddPosition(animation.positionStep);
            transform->AddRotation(animation.rotationStep);
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

            uint64 frame = Core::GetInstance().GetFrame();
            float frameLength = 8;

            vec3 finalPostion = moveEvent.position;
            vec3 finalRotation = moveEvent.rotation;
            vec3 positionStep;
            vec3 rotationStep;
            if (moveEvent.positionRelative) {
                finalPostion += transform.GetPosition();
                positionStep = moveEvent.position / frameLength;
            }
            else {
                positionStep = (moveEvent.position - transform.GetPosition()) / frameLength;
            }

            if (moveEvent.rotationRelative) {
                finalRotation += transform.GetRotation();
                rotationStep = moveEvent.rotation / frameLength;
            }
            else {
                float angle = FindTurningAngle(transform.GetRotation().y, moveEvent.rotation.y);
                rotationStep.y = angle;
                rotationStep /= frameLength;
            }

            TransformAnimation animation;
            animation.id = moveEvent.entity;
            animation.endFrame = frame + (uint64)frameLength;
            animation.finalPostion = finalPostion;
            animation.finalRotation = finalRotation;
            animation.positionStep = positionStep;
            animation.rotationStep = rotationStep;
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