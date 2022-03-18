#include "GameplayManager.h"

#include "Core.h"

namespace Plasmium {
    void GameplayManager::LoadLevelFile(FileResource levelFile)
    {
        auto & resourceManager = Core::GetInstance().GetResourceManager();
        currentLevel = &resourceManager.GetStaticLevelResource(levelFile);
        currentLevel->Load();
    }

    void GameplayManager::ReloadCurrentLevel() {
        currentLevel->Unload();
        currentLevel->Load();
    }

    void GameplayManager::ProcessEvent(const GenericEvent& event)
    {
        if ((EventType)event.index() == EventType::Input) {
            auto& inputEvent = std::get<InputEvent>(event);
            for (auto& pcc : playerControllerComponents.GetObjectsReference()) {
                pcc.ProcessInput(inputEvent);
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
        if ((EventType)event.index() == EventType::AttackEntity) {
            auto& attackEvent = std::get<AttackEntityEvent>(event);
            auto& attackerCombat = *combatComponents.GetObjectPtr(attackEvent.attackerId);
            auto& defenderCombat = *combatComponents.GetObjectPtr(attackEvent.defenderId);
            auto& attackerName = *nameComponents.GetObjectPtr(attackEvent.attackerId);
            auto& defenderName = *nameComponents.GetObjectPtr(attackEvent.defenderId);

            defenderCombat.DoDamage(attackerCombat.GetDamage());

            char buffer[256];
            sprintf_s(buffer, "%s attacked %s for %i damage!",
                attackerName.GetName(),
                defenderName.GetName(),
                (int32)attackerCombat.GetDamage());

            auto& resourceManager = Core::GetInstance().GetResourceManager();
            StringId attackString = resourceManager.CreateString(buffer);
            Core::GetInstance().PostEvent(GameplayEventLogEvent(attackString));

            if (defenderCombat.GetHealth() <= 0.0f) {
                Core::GetInstance().PostEvent(EntityKilledEvent(
                    attackEvent.attackerId,
                    attackEvent.defenderId));

                sprintf_s(buffer, "%s slew %s!",
                    attackerName.GetName(),
                    defenderName.GetName());
                StringId killString = resourceManager.CreateString(buffer);
                Core::GetInstance().PostEvent(GameplayEventLogEvent(killString));
            }
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

    void GameplayManager::CreateComponent(const ComponentCreationArgs& creationArgs,
        float health,
        float damage) {
        assert(creationArgs.type == ComponentType::Combat);
        combatComponents.EmplaceObject(creationArgs.parent,
            CombatComponent(creationArgs, health, damage));
    }

    void GameplayManager::CreateComponent(const ComponentCreationArgs& creationArgs,
        const char* name) {
        assert(creationArgs.type == ComponentType::Name);
        nameComponents.EmplaceObject(creationArgs.parent,
            NameComponent(creationArgs, name));
    }

    void GameplayManager::PreDeleteComponent(EntityId id, ComponentType type)
    {
        auto& transform = *Core::GetInstance().GetEntityManager().GetTransform(id);
        vec3 logicalPosition = transform.GetLogicalPosition();
        currentLevel->ClearCreature(logicalPosition);
    }

    void GameplayManager::DeleteComponent(EntityId id, ComponentType type)
    {
        assert(type == ComponentType::PlayerController
            || type == ComponentType::MonsterController
            || type == ComponentType::Combat
            || type == ComponentType::Name);
        if (type == ComponentType::PlayerController) {
            playerControllerComponents.DeleteObject(id);
        }
        else if (type == ComponentType::MonsterController) {
            monsterControllerComponents.DeleteObject(id);
        }
        else if (type == ComponentType::Combat) {
            combatComponents.DeleteObject(id);
        }
        else if (type == ComponentType::Name) {
            nameComponents.DeleteObject(id);
        }
    }
}