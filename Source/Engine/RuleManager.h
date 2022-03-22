#pragma once
#include "Array.h"
#include "GameplayTypes.h"
#include "Types.h"

namespace Plasmium {
    class RuleManager {
    private:
        EntityId playerId;
        FactionType currentTurn = FactionType::Player;
        bool playerIsActing = false;
        Array<EntityId> monstersToAct;
        bool IsPlayer(EntityId id) { return id == playerId; }
        bool ActPassesTurn(ActionType actionType);

    public:
        bool EntityCanAct(EntityId id);
        void SetPlayer(EntityId id) { playerId = id; }
        void ActCompleted(EntityId id, ActionType actionType);
    };
}