#include "ControllerComponent.h"
#include "Core.h"
#include "Event.h"

namespace Plasmium {
    void ControllerComponent::TryMoveOrAttack(Direction direction)
    {
        vec3 logicalMovement = vec3();
        if (direction == Direction::N || direction == Direction::NE || direction == Direction::NW) {
            logicalMovement.z = -1;
        }
        else if (direction == Direction::S || direction == Direction::SE || direction == Direction::SW) {
            logicalMovement.z = 1;
        }

        if (direction == Direction::E || direction == Direction::NE || direction == Direction::SE) {
            logicalMovement.x = 1;
        }
        else if (direction == Direction::W || direction == Direction::NW || direction == Direction::SW) {
            logicalMovement.x = -1;
        }

        auto& transform = *Core::GetInstance().GetEntityManager().GetTransform(GetId());
        auto& gameplayManager = Core::GetInstance().GetGameplayManager();

        vec3 logicalDestination = transform.GetLogicalPosition() + logicalMovement;
        if (gameplayManager.HasCreature(logicalDestination)) {
            EntityId enemy = gameplayManager.GetCreature(logicalDestination);
            vec3 rotation = vec3(0.0f, (int32)direction * 45.0f, 0.0f);
            Core::GetInstance().PostEvent(AttackEntityEvent(GetId(), enemy, rotation));
        }
        else {
            vec3 rotation = vec3(0.0f, (int32)direction * 45.0f, 0.0f);
            Core::GetInstance().PostEvent(MoveEntityEvent(GetId(),
                logicalMovement,
                TransformComponent::LogicalPointToWorld(logicalMovement),
                MovementType::Relative,
                rotation,
                MovementType::Absolute));
        }
    }
}
