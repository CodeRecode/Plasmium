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

        Core::GetInstance().PostEvent(TryMoveEntityEvent(GetId(), logicalMovement, direction));
    }
}
