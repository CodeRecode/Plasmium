#include "PlayerControllerComponent.h"
#include "InputTypes.h"

namespace Plasmium {
    void PlayerControllerComponent::ProcessInput(const InputEvent& event)
    {
        if (event.GetKeyToggleDown(InputKey::W)) {
            TryMoveOrAttack(Direction::N);
        }
        else if (event.GetKeyToggleDown(InputKey::A)) {
            TryMoveOrAttack(Direction::W);
        }
        else if (event.GetKeyToggleDown(InputKey::X)) {
            TryMoveOrAttack(Direction::S);
        }
        else if (event.GetKeyToggleDown(InputKey::D)) {
            TryMoveOrAttack(Direction::E);
        }
        else if (event.GetKeyToggleDown(InputKey::Q)) {
            TryMoveOrAttack(Direction::NW);
        }
        else if (event.GetKeyToggleDown(InputKey::E)) {
            TryMoveOrAttack(Direction::NE);
        }
        else if (event.GetKeyToggleDown(InputKey::Z)) {
            TryMoveOrAttack(Direction::SW);
        }
        else if (event.GetKeyToggleDown(InputKey::C)) {
            TryMoveOrAttack(Direction::SE);
        }
    }
}