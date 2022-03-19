#pragma once
#include "Animation.h"
#include "GameplayTypes.h"
#include "Types.h"

namespace Plasmium {
    class RuleManager {
    private:
        EntityId playerId;
        FactionType currentTurn = FactionType::Player;
        bool playerIsActing = false;
        bool IsPlayer(EntityId id) { return id == playerId; }

    public:
        bool EntityCanAct(EntityId id);
        void SetPlayer(EntityId id) { playerId = id; }
        void ActCompleted(EntityId id, AnimationType animationType);
    };
}