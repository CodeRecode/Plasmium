#pragma once

#include "Component.h"
#include "CameraComponent.h"
#include "CombatComponent.h"
#include "ModelComponent.h"
#include "MonsterControllerComponent.h"
#include "NameComponent.h"
#include "PlayerControllerComponent.h"
#include "TransformComponent.h"

namespace Plasmium {
    ComponentType CameraComponent::type = ComponentType::Camera;
    ComponentType CombatComponent::type = ComponentType::Combat;
    ComponentType ModelComponent::type = ComponentType::Model;
    ComponentType MonsterControllerComponent::type = ComponentType::MonsterController;
    ComponentType NameComponent::type = ComponentType::Name;
    ComponentType PlayerControllerComponent::type = ComponentType::PlayerController;
    ComponentType TransformComponent::type = ComponentType::Transform;
}