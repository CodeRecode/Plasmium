#pragma once
#include "Component.h"
#include "PlasString.h"

namespace Plasmium {
    class NameComponent : public Component {
    private:
        PlasString name;

    public:
        NameComponent(const ComponentCreationArgs& args, const char* name) :
            Component(args),
            name(name)
        {
        }
        const char* GetName() const { return name.Get(); }
    };
}
