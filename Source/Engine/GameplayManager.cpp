#include "GameplayManager.h"

#include "Core.h"

namespace Plasmium {
    void GameplayManager::LoadLevelFile(FileResource levelFile)
    {
        auto & resourceManager = Core::GetInstance().GetResourceManager();
        currentLevel = &resourceManager.GetStaticLevelResource(levelFile);
        currentLevel->Load();
    }

    void GameplayManager::ProcessEvent(const GenericEvent& event)
    {
        if ((EventType)event.index() == EventType::Input) {
            auto& inputEvent = std::get<InputEvent>(event);
            for (auto& pcc : playerControllerComponents.GetObjectsReference()) {
                pcc.ProcessInput(inputEvent);
            }

            if (inputEvent.GetKeyDown(InputKey::F9)) {
                currentLevel->Unload();
                currentLevel->Load();
            }
        }
        if ((EventType)event.index() == EventType::EntityCreated) {
            auto& entityCreated = std::get<EntityCreatedEvent>(event);
            EntityId id = entityCreated.entityId;
            if (playerControllerComponents.Contains(id)
                || monsterControllerComponents.Contains(id)) {
                currentLevel->SetCreature(id, entityCreated.logicalPosition);
            }
        }
        if ((EventType)event.index() == EventType::MoveEntityComplete) {
            auto& entityMoved = std::get<MoveEntityCompleteEvent>(event);
            currentLevel->SetCreature(entityMoved.entityId, entityMoved.logicalPositionEnd);
            currentLevel->ClearCreature(entityMoved.logicalPositionStart);
        }
    }

    void GameplayManager::CreateComponent(const ComponentCreationArgs& creationArgs)
    {
        assert(creationArgs.type == ComponentType::PlayerController || 
            creationArgs.type == ComponentType::MonsterController);
        if (creationArgs.type == ComponentType::PlayerController) {
            playerControllerComponents.EmplaceObject(creationArgs.parent,
                PlayerControllerComponent(creationArgs));
        }
        else if (creationArgs.type == ComponentType::MonsterController) {
            monsterControllerComponents.EmplaceObject(creationArgs.parent,
                MonsterControllerComponent(creationArgs));
        }
    }

    void GameplayManager::PreDeleteComponent(EntityId id, ComponentType type)
    {
        auto& transform = *Core::GetInstance().GetEntityManager().GetTransform(id);
        vec3 logicalPosition = transform.GetLogicalPosition();
        currentLevel->ClearCreature(logicalPosition);
    }

    void GameplayManager::DeleteComponent(EntityId id, ComponentType type)
    {
        assert(type == ComponentType::PlayerController ||
            type == ComponentType::MonsterController);
        if (type == ComponentType::PlayerController) {
            playerControllerComponents.DeleteObject(id);
        }
        else if (type == ComponentType::MonsterController) {
            monsterControllerComponents.DeleteObject(id);
        }
    }
}