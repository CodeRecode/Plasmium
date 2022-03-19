#include "RuleManager.h"

namespace Plasmium {
    bool RuleManager::EntityCanAct(EntityId id)
    {
        if (!IsPlayer(id)) {
            return currentTurn == FactionType::Monster;
        }
        if (currentTurn != FactionType::Player || playerIsActing) {
            return false;
        }

        playerIsActing = true;
        return true;
    }

    void RuleManager::ActCompleted(EntityId id, AnimationType animationType)
    {
        if (!IsPlayer(id)) {
            // todo
            return;
        }
        playerIsActing = false;
        if (animationType == AnimationType::Attack ||
            animationType == AnimationType::Walk) {
            // todo
            // currentTurn = FactionType::Monster;
        }
    }
}