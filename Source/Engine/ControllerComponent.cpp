#include "ControllerComponent.h"
#include "Core.h"
#include "Event.h"

namespace Plasmium {
    void ControllerComponent::TryMoveOrAttack(Direction direction)
    {
        float xDir = 0;
        float zDir = 0;
        if (direction == Direction::N || direction == Direction::NE || direction == Direction::NW) {
            zDir = 2;
        }
        if (direction == Direction::S || direction == Direction::SE || direction == Direction::SW) {
            zDir = -2;
        }

        if (direction == Direction::E || direction == Direction::NE || direction == Direction::SE) {
            xDir = 2;
        }
        if (direction == Direction::W || direction == Direction::NW || direction == Direction::SW) {
            xDir = -2;
        }

        vec3 relativePosition = vec3(xDir, 0.0f, zDir);
        auto& entityManager = Core::GetInstance().GetEntityManager();
        vec3 currentPostion = entityManager.GetTransform(GetId())->GetPosition();

        const auto& levelManager = Core::GetInstance().GetLevelManager();
        vec3 logicalDestination = TransformComponent::WorldPointToLogical(currentPostion + relativePosition);
        if (!levelManager.IsWalkable(logicalDestination)) {
            return;
        }

        vec3 rotation = vec3(0.0f, (int32)direction * 45.0f, 0.0f);
        Core::GetInstance().PostEvent(MoveEntityEvent(GetId(), relativePosition, rotation, true, false));
    }
}
