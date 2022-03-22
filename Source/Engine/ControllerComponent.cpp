#include "ControllerComponent.h"
#include "Core.h"
#include "Event.h"
#include "Plasmath.h"

namespace Plasmium {
    void ControllerComponent::TryMoveOrAttack(Direction direction)
    {
        Core::PostEvent(TryMoveEntityEvent(GetIdInternal(), DirectionToVector(direction)));
    }
}
