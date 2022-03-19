#pragma once
#include "ControllerComponent.h"
#include "Event.h"

namespace Plasmium {
    class PlayerControllerComponent :
        public ControllerComponent,
        public Component<PlayerControllerComponent>
    {
    private:
        EntityId GetIdInternal() const override { return GetId(); }
    public:
        PlayerControllerComponent(EntityId entityId) :
            Component(entityId)
        {}
        void ProcessInput(const InputEvent& event);
    };
    ComponentType PlayerControllerComponent::type = ComponentType::PlayerController;
}
