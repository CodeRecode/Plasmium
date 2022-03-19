#pragma once
#include "CoreSystem.h"
#include "Event.h"
#include "Level.h"
#include "RuleManager.h"

namespace Plasmium {
    class GameplayManager : public CoreSystem {
    private:
        Level* currentLevel;
        RuleManager ruleManager;

    public:
        void LoadLevelFile(FileResource levelFile);
        void ReloadCurrentLevel();
        void ProcessEvent(const GenericEvent& event) override;

        bool IsWalkable(vec3 logicalPostion) const { return currentLevel->IsWalkable(logicalPostion); }
        bool HasCreature(vec3 logicalPostion) const { return currentLevel->HasCreature(logicalPostion); }
        EntityId GetCreature(vec3 logicalPostion) const { return currentLevel->GetCreature(logicalPostion); }
    };
}