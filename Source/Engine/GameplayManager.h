#pragma once
#include "CoreSystem.h"
#include "CombatComponent.h"
#include "ComponentManager.h"
#include "Event.h"
#include "Level.h"
#include "MonsterControllerComponent.h"
#include "NameComponent.h"
#include "PlayerControllerComponent.h"

namespace Plasmium {
    class GameplayManager : public CoreSystem, public ComponentManager {
    private:
        Level* currentLevel; 
        Handler<EntityId, PlayerControllerComponent> playerControllerComponents;
        Handler<EntityId, MonsterControllerComponent> monsterControllerComponents;
        Handler<EntityId, CombatComponent> combatComponents;
        Handler<EntityId, NameComponent> nameComponents;

    public:
        void LoadLevelFile(FileResource levelFile);
        void ReloadCurrentLevel();
        void ProcessEvent(const GenericEvent& event) override;

        bool IsWalkable(vec3 logicalPostion) const { return currentLevel->IsWalkable(logicalPostion); }
        bool HasCreature(vec3 logicalPostion) const { return currentLevel->HasCreature(logicalPostion); }
        EntityId GetCreature(vec3 logicalPostion) const { return currentLevel->GetCreature(logicalPostion); }


        void CreateComponent(const ComponentCreationArgs& creationArgs) override;
        void CreateComponent(const ComponentCreationArgs& creationArgs, 
            float health, 
            float damage) override;
        void CreateComponent(const ComponentCreationArgs& creationArgs,
            const char* name) override;
        void PreDeleteComponent(EntityId id, ComponentType type) override;
        void DeleteComponent(EntityId id, ComponentType type) override;
    };
}