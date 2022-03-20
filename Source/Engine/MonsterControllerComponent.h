#pragma once
#include "ControllerComponent.h"
#include "Event.h"

namespace Plasmium {
    class MonsterControllerComponent : 
        public ControllerComponent, 
        public Component<MonsterControllerComponent>
    {
    private:
        EntityId GetIdInternal() const override { return GetId(); }
    public:
        MonsterControllerComponent(EntityId entityId) :
            Component(entityId)
        {}

        void Act();
    };
}