#pragma once
#include "Array.h"

namespace Plasmium {
    enum class ComponentType {
        Transform,
        Model,
        PlayerController,
        MonsterController,
        Combat,
        Name,
        ComponentTypeCount
    };

    struct ComponentCreationArgs {
        ComponentType type;
        EntityId parent;
        ComponentCreationArgs(ComponentType type, EntityId parent) :
            type(type),
            parent(parent) 
        {}
    };

    class Component {
    private:
        ComponentType type;
        EntityId parent;
    public:
        Component(const ComponentCreationArgs& args) :
            type(args.type),
            parent(args.parent)
        {}

        EntityId GetId() const { return parent; }
    };
}