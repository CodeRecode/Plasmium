#pragma once
#include "CoreSystem.h"
#include "Event.h"
#include "Level.h"

namespace Plasmium {
    class LevelManager : public CoreSystem {
    private:
        Level* currentLevel;
    public:
        void LoadLevelFile(FileResource levelFile);
        void ProcessEvent(const GenericEvent& event) override;
        bool IsWalkable(vec3 logicalPostion) const { return currentLevel->IsWalkable(logicalPostion); }
    };
}