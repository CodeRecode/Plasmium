#pragma once
#include "Component.h"
#include "PlasString.h"

namespace Plasmium {
    class NameComponent : public Component<NameComponent> {
    private:
        PlasString name;

    public:
        NameComponent(EntityId entityId, const char* name) :
            Component(entityId),
            name(name)
        {
        }
        const char* GetName() const { return name.Get(); }
    };
}
