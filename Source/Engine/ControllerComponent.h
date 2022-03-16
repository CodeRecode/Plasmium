#pragma once
#include "Types.h"
#include "Component.h"

namespace Plasmium {
    class ControllerComponent : public Component
    {
    private:
    public:
        virtual ~ControllerComponent() {};

        ControllerComponent(const ComponentCreationArgs& args) : Component(args) {}
        void TryMoveOrAttack(Direction direction);
    };
}
