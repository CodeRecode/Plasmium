#include "MonsterControllerComponent.h"

#include "Core.h"
#include "EntityManager.h"
#include "Plasmath.h"
#include "PlayerControllerComponent.h"
#include "TransformComponent.h"

namespace Plasmium {
    void MonsterControllerComponent::Act()
    {
        auto& transform = *Core::GetEntityManager().GetComponent<TransformComponent>(GetId());

        auto pccArray = Core::GetEntityManager().GetComponentArray<PlayerControllerComponent>();
        if (!pccArray.Empty()) {
            auto& pcc = pccArray[0]; 
            auto& playerTransform = *Core::GetEntityManager().GetComponent<TransformComponent>(pcc.GetId());
            vec3 logicalMove = playerTransform.GetLogicalPosition() - transform.GetLogicalPosition();
            float manhattanDistance = fabs(logicalMove.x) + fabs(logicalMove.y) + fabs(logicalMove.z);

            // Move towards the player
            if (manhattanDistance <= 3) {
                logicalMove.x = Clamp<float>(logicalMove.x, -1.0f, 1.0f);
                logicalMove.z = Clamp<float>(logicalMove.z, -1.0f, 1.0f);

                Core::PostEvent(TryMoveEntityEvent(GetIdInternal(), logicalMove));
                return;
            }
        }

        uint32 decisionWeight = Core::GetNextRandom() % 100;

        if (decisionWeight >= 75) {
            Core::PostEvent(PassTurnEvent(GetId()));
        }
        else if (decisionWeight >= 25) {
            // Turn
            Direction turnDirection = (Direction)(Core::GetNextRandom() %
                (uint32)Direction::DirectionCount);

            AnimateEntityParameters animationParams;
            animationParams.entityId = GetId();
            animationParams.startPosition = transform.GetPosition();
            animationParams.endPosition = transform.GetPosition();
            animationParams.startRotation = transform.GetRotation();
            animationParams.endRotation = DirectionToRotation(turnDirection);
            animationParams.type = AnimationType::Turn;
            Core::PostEvent(AnimateEntityEvent(std::move(animationParams)));
        }
        else {
            // Wander
            Direction wanderDirection = (Direction)(Core::GetNextRandom() %
                (uint32)Direction::DirectionCount);
            TryMoveOrAttack(wanderDirection);
        }
    }
}