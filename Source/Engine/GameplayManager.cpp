#include "GameplayManager.h"

#include "Core.h"
#include "CombatComponent.h"
#include "EntityManager.h"
#include "PlayerControllerComponent.h"
#include "MonsterControllerComponent.h"
#include "NameComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"

namespace Plasmium {
    void GameplayManager::LoadLevelFile(FileResource levelFile)
    {
        auto & resourceManager = Core::GetResourceManager();
        currentLevel = &resourceManager.GetStaticLevelResource(levelFile);
        currentLevel->Load();
    }

    void GameplayManager::ReloadCurrentLevel() {
        currentLevel->Unload();
        currentLevel->Load();
    }

    void GameplayManager::ProcessEvent(const GenericEvent& event)
    {
        if ((EventType)event.index() == EventType::Input) {
            auto& inputEvent = std::get<InputEvent>(event);
            for (auto& pcc : Core::GetEntityManager().GetComponentArray<PlayerControllerComponent>()) {
                pcc.ProcessInput(inputEvent);
            }
        }
        if ((EventType)event.index() == EventType::EntityCreated) {
            auto& entityCreated = std::get<EntityCreatedEvent>(event);
            EntityId id = entityCreated.entityId;
            bool isPlayer = Core::GetEntityManager().GetComponent<PlayerControllerComponent>(id) != nullptr;
            bool isMonster = Core::GetEntityManager().GetComponent<MonsterControllerComponent>(id) != nullptr;

            auto* combat = Core::GetEntityManager().GetComponent<CombatComponent>(id);

            if (isPlayer || isMonster) {
                currentLevel->SetCreature(id, entityCreated.logicalPosition);
            }
            if (isPlayer) {
                ruleManager.SetPlayer(id);
                combat->SetFaction(FactionType::Player);
            }
        }
        if ((EventType)event.index() == EventType::TryMoveEntity) {
            auto& entityMove = std::get<TryMoveEntityEvent>(event);
            auto& transform = *Core::GetEntityManager().GetComponent<TransformComponent>(entityMove.entityId);
            vec3 targetRotation = vec3(0.0f, (int32)entityMove.direction * 45.0f, 0.0f);

            if (!ruleManager.EntityCanAct(entityMove.entityId)) {
                return;
            }

            AnimateEntityParameters animationParams;
            animationParams.entityId = entityMove.entityId;
            animationParams.startPosition = transform.GetPosition();
            animationParams.endPosition = transform.GetPosition();
            animationParams.startRotation = transform.GetRotation();
            animationParams.endRotation = targetRotation;

            vec3 logicalDestination = transform.GetLogicalPosition() + entityMove.relativeLogicalPosition;
            if (HasCreature(logicalDestination)) {
                EntityId creature = GetCreature(logicalDestination);
                auto* combatComponent = Core::GetEntityManager().GetComponent<CombatComponent>(creature);
                if (combatComponent != nullptr) {
                    animationParams.type = AnimationType::Attack;
                    animationParams.targetId = creature;
                    Core::PostEvent(AnimateEntityEvent(std::move(animationParams)));
                    return;
                }
            }
            
            if (!IsWalkable(logicalDestination)) {
                animationParams.type = AnimationType::Bump;
                Core::PostEvent(AnimateEntityEvent(std::move(animationParams)));
            }
            else {
                vec3 worldTarget = TransformComponent::LogicalPointToWorld(logicalDestination);
                animationParams.type = AnimationType::Walk;
                animationParams.endPosition = worldTarget;
                Core::PostEvent(AnimateEntityEvent(std::move(animationParams)));

                // Update logical postion immediately to avoid passing it through the animation
                currentLevel->SetCreature(entityMove.entityId, logicalDestination);
                currentLevel->ClearCreature(transform.GetLogicalPosition()); 
                Core::PostEvent(ChangeTransformEvent(
                    entityMove.entityId,
                    ChangeTransformLogicalPosition,
                    logicalDestination,
                    vec3(),
                    vec3(),
                    vec3()));

            }
        }
        if ((EventType)event.index() == EventType::AnimateEntityKey) {
            auto& animationKey = std::get<AnimateEntityKeyEvent>(event).params;
            EntityId entityId = animationKey.animationParams.entityId;
            if (animationKey.keyType == AnimationKeyType::Attack) {
                Core::PostEvent(AttackEntityEvent(
                    entityId,
                    animationKey.animationParams.targetId));
            }
            else if (animationKey.keyType == AnimationKeyType::AnimationComplete) {
                if (animationKey.animationParams.type == AnimationType::Death) {
                    Core::PostEvent(DestroyEntityEvent(entityId));
                }
                ruleManager.ActCompleted(entityId, animationKey.animationParams.type);
            }
        }
        if ((EventType)event.index() == EventType::AttackEntity) {
            auto& attackEvent = std::get<AttackEntityEvent>(event);
            auto* attackerCombat = Core::GetEntityManager().GetComponent<CombatComponent>(attackEvent.attackerId);
            auto* defenderCombat = Core::GetEntityManager().GetComponent<CombatComponent>(attackEvent.defenderId);
            auto& attackerName = *Core::GetEntityManager().GetComponent<NameComponent>(attackEvent.attackerId);
            auto& defenderName = *Core::GetEntityManager().GetComponent<NameComponent>(attackEvent.defenderId);

            if (attackerCombat == nullptr || defenderCombat == nullptr) {
                // Participant may have died
                return;
            }

            defenderCombat->DoDamage(attackerCombat->GetDamage());

            char buffer[256];
            sprintf_s(buffer, "%s attacked %s for %i damage!",
                attackerName.GetName(),
                defenderName.GetName(),
                (int32)attackerCombat->GetDamage());

            auto& resourceManager = Core::GetResourceManager();
            StringId attackString = resourceManager.CreateString(buffer);
            Core::PostEvent(GameplayEventLogEvent(attackString));

            if (defenderCombat->GetHealth() <= 0.0f) {
                Core::PostEvent(EntityKilledEvent(
                    attackEvent.attackerId,
                    attackEvent.defenderId));

                sprintf_s(buffer, "%s slew %s!",
                    attackerName.GetName(),
                    defenderName.GetName());
                StringId killString = resourceManager.CreateString(buffer);
                Core::PostEvent(GameplayEventLogEvent(killString));

                auto& transform = *Core::GetEntityManager().GetComponent<TransformComponent>(
                    attackEvent.defenderId);

                AnimateEntityParameters animationParams;
                animationParams.type = AnimationType::Death;
                animationParams.entityId = attackEvent.defenderId;
                animationParams.startPosition = transform.GetPosition();
                animationParams.endPosition = transform.GetPosition();
                animationParams.startRotation = transform.GetRotation();
                animationParams.endRotation = transform.GetRotation();
                Core::PostEvent(AnimateEntityEvent(std::move(animationParams)));

                Core::PostEvent(DestroyComponentEvent(attackEvent.defenderId, 
                    ComponentType::Combat));
                currentLevel->ClearCreature(transform.GetLogicalPosition());
            }
        }
    }
}