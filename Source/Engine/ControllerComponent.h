#pragma once
#include "Component.h"
#include "Types.h"

namespace Plasmium {
    class ControllerComponent
    {
    protected:
        virtual EntityId GetIdInternal() const = 0;
    public:
        virtual ~ControllerComponent() {};

        void TryMoveOrAttack(Direction direction);
    };
}
