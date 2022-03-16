#pragma once
#include "ControllerComponent.h"
#include "Event.h"

namespace Plasmium {
    class MonsterControllerComponent : public ControllerComponent {
    private:
    public:
        MonsterControllerComponent(const ComponentCreationArgs& args) :
            ControllerComponent(args)
        {}
    };
}