#pragma once
#include "Array.h"
#include "HashFunctions.h"

namespace Plasmium {
    enum class ComponentType {
        Camera,
        Combat,
        Model,
        MonsterController,
        Name,
        PlayerController,
        Transform,
        ComponentTypeCount
    };

    template<class T>
    class Component {
    private:
        static ComponentType type;
        EntityId entityId;
    public:
        Component(EntityId entityId) :
            entityId(entityId)
        {}
        EntityId GetId() const { return entityId; }

        static ComponentType GetType() { return type; }
    };
}

template <>
inline uint32 HashFunc<Plasmium::ComponentType>(Plasmium::ComponentType value) {
    return (uint32)value;
}
