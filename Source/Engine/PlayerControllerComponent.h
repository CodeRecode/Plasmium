#pragma once
#include "ControllerComponent.h"
#include "Event.h"

namespace Plasmium {
    class PlayerControllerComponent : public ControllerComponent
    {
    private:
    public:
        PlayerControllerComponent(const ComponentCreationArgs& args) :
            ControllerComponent(args)
        {}
        void ProcessInput(const InputEvent& event);
    };
}
