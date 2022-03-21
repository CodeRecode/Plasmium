#include "RuleManager.h"

#include "CombatComponent.h"
#include "Core.h"
#include "EntityManager.h"
#include "MonsterControllerComponent.h"

namespace Plasmium {
    bool RuleManager::ActPassesTurn(AnimationType animationType) {
        return animationType != AnimationType::Bump
            && animationType != AnimationType::Death;
    }

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
            while (!monstersToAct.Empty()) {
                EntityId entityId = monstersToAct.Pop();
                auto* mcc = Core::GetEntityManager().GetComponent<MonsterControllerComponent>(entityId);

                if (mcc == nullptr) {
                    continue; // Monster was destroyed
                }
                mcc->Act();
                return;
            }
            // No monster was found to act
            currentTurn = FactionType::Player;

            return;
        }

        playerIsActing = false;
        if (ActPassesTurn(animationType)) {
            currentTurn = FactionType::Monster;
            auto& monsters = Core::GetEntityManager().GetComponentArray<MonsterControllerComponent>();
            for (auto& monster : monsters) {
                auto* combat = Core::GetEntityManager().GetComponent<CombatComponent>(monster.GetId());
                if (combat != nullptr && !combat->IsDead()) {
                    monstersToAct.Push(monster.GetId());
                }
            }

            // No valid monster was found to act
            if (monstersToAct.Empty()) {
                currentTurn = FactionType::Player;
                return;
            }

            EntityId entityId = monstersToAct.Pop();
            auto& mcc = *Core::GetEntityManager().GetComponent<MonsterControllerComponent>(entityId);
            mcc.Act();
        }
    }
}