#pragma once
#include "Array.h"
#include "Component.h"

namespace Plasmium {
    class Entity {
    private:
        EntityId id;
        Array<uint32> componentTypes;
    public:
        Entity(EntityId id) : id(id) {}

        void AddComponentType(ComponentType id) { componentTypes.Push((uint32)id); }
        const Array<uint32>& GetComponentTypes() { return componentTypes; }
        EntityId GetId() const { return id; }
    };
}