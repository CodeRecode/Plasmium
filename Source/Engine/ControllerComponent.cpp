#include "ControllerComponent.h"
#include "Core.h"
#include "Event.h"

namespace Plasmium {
    void ControllerComponent::TryMoveOrAttack(Direction direction)
    {
        vec3 logicalPostion = vec3();
        if (direction == Direction::N || direction == Direction::NE || direction == Direction::NW) {
            logicalPostion.z = -1;
        }
        else if (direction == Direction::S || direction == Direction::SE || direction == Direction::SW) {
            logicalPostion.z = 1;
        }

        if (direction == Direction::E || direction == Direction::NE || direction == Direction::SE) {
            logicalPostion.x = 1;
        }
        else if (direction == Direction::W || direction == Direction::NW || direction == Direction::SW) {
            logicalPostion.x = -1;
        }

        vec3 rotation = vec3(0.0f, (int32)direction * 45.0f, 0.0f);
        Core::GetInstance().PostEvent(MoveEntityEvent(GetId(),
            logicalPostion,
            TransformComponent::LogicalPointToWorld(logicalPostion),
            MovementType::Relative,
            rotation,
            MovementType::Absolute));
    }
}
