#pragma once
#include "CoreSystem.h"
#include "ComponentManager.h"
#include "Event.h"
#include "Level.h"
#include "MonsterControllerComponent.h"
#include "PlayerControllerComponent.h"

namespace Plasmium {
    class GameplayManager : public CoreSystem, public ComponentManager {
    private:
        Level* currentLevel;
        Handler<EntityId, PlayerControllerComponent> playerControllerComponents;
        Handler<EntityId, MonsterControllerComponent> monsterControllerComponents;

    public:
        void LoadLevelFile(FileResource levelFile);
        void ProcessEvent(const GenericEvent& event) override;

        bool IsWalkable(vec3 logicalPostion) const { return currentLevel->IsWalkable(logicalPostion); }

        void CreateComponent(const ComponentCreationArgs& creationArgs) override;
        void PreDeleteComponent(EntityId id, ComponentType type) override;
        void DeleteComponent(EntityId id, ComponentType type) override;
    };
}