#include "PlayerControllerComponent.h"
#include "InputTypes.h"

namespace Plasmium {
    void PlayerControllerComponent::ProcessInput(const InputEvent& event)
    {
        if (event.GetKeyToggleDown(KeybindFunction::MovePlayerN)) {
            TryMoveOrAttack(Direction::N);
        }
        else if (event.GetKeyToggleDown(KeybindFunction::MovePlayerW)) {
            TryMoveOrAttack(Direction::W);
        }
        else if (event.GetKeyToggleDown(KeybindFunction::MovePlayerS)) {
            TryMoveOrAttack(Direction::S);
        }
        else if (event.GetKeyToggleDown(KeybindFunction::MovePlayerE)) {
            TryMoveOrAttack(Direction::E);
        }
        else if (event.GetKeyToggleDown(KeybindFunction::MovePlayerNW)) {
            TryMoveOrAttack(Direction::NW);
        }
        else if (event.GetKeyToggleDown(KeybindFunction::MovePlayerNE)) {
            TryMoveOrAttack(Direction::NE);
        }
        else if (event.GetKeyToggleDown(KeybindFunction::MovePlayerSW)) {
            TryMoveOrAttack(Direction::SW);
        }
        else if (event.GetKeyToggleDown(KeybindFunction::MovePlayerSE)) {
            TryMoveOrAttack(Direction::SE);
        }
    }
}