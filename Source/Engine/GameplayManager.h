#pragma once
#include "CoreSystem.h"
#include "Event.h"
#include "Level.h"
#include "RuleManager.h"

namespace Plasmium {
    class GameplayManager : public CoreSystem {
    private:
        Level* currentLevel = nullptr;
        RuleManager ruleManager;

    public:

        bool IsWalkable(vec3 logicalPostion, vec3 relativeMove) const;
        bool HasCreature(vec3 logicalPostion) const { return currentLevel->HasCreature(logicalPostion); }
        EntityId GetCreature(vec3 logicalPostion) const { return currentLevel->GetCreature(logicalPostion); }

        void LoadLevelFile(FileResource levelFile);
        void ReloadCurrentLevel();
        void ProcessEvent(const GenericEvent& event) override;
    };
}