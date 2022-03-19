#include "GameplayManager.h"

#include "Core.h"
#include "EntityManager.h"
#include "ResourceManager.h"

namespace Plasmium {
    void GameplayManager::LoadLevelFile(FileResource levelFile)
    {
        auto & resourceManager = Core::GetInstance().GetResourceManager();
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
            for (auto& pcc : playerControllerComponents.GetObjectsReference()) {
                pcc.ProcessInput(inputEvent);
            }
        }
        if ((EventType)event.index() == EventType::EntityCreated) {
            auto& entityCreated = std::get<EntityCreatedEvent>(event);
            EntityId id = entityCreated.entityId;
            if (playerControllerComponents.Contains(id)
                || monsterControllerComponents.Contains(id)) {
                currentLevel->SetCreature(id, entityCreated.logicalPosition);
            }
        }
        if ((EventType)event.index() == EventType::TryMoveEntity) {
            auto& entityMove = std::get<TryMoveEntityEvent>(event);
            auto& transform = *Core::GetInstance().GetEntityManager().GetTransform(entityMove.entityId);
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
                auto* combatComponent = combatComponents.GetObjectPtr(creature);
                if (combatComponent != nullptr) {
                    animationParams.type = AnimationType::Attack;
                    animationParams.targetId = creature;
                    Core::GetInstance().PostEvent(AnimateEntityEvent(std::move(animationParams)));
                    return;
                }
            }
            
            if (!IsWalkable(logicalDestination)) {
                animationParams.type = AnimationType::Bump;
                Core::GetInstance().PostEvent(AnimateEntityEvent(std::move(animationParams)));
            }
            else {
                vec3 worldTarget = TransformComponent::LogicalPointToWorld(logicalDestination);
                animationParams.type = AnimationType::Walk;
                animationParams.endPosition = worldTarget;
                Core::GetInstance().PostEvent(AnimateEntityEvent(std::move(animationParams)));

                // Update logical postion immediately to avoid passing it through the animation
                currentLevel->SetCreature(entityMove.entityId, logicalDestination);
                currentLevel->ClearCreature(transform.GetLogicalPosition()); 
                Core::GetInstance().PostEvent(ChangeTransformEvent(
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
                Core::GetInstance().PostEvent(AttackEntityEvent(
                    entityId,
                    animationKey.animationParams.targetId));
            }
            else if (animationKey.keyType == AnimationKeyType::AnimationComplete) {
                if (animationKey.animationParams.type == AnimationType::Death) {
                    Core::GetInstance().PostEvent(DestroyEntityEvent(entityId));
                }
                ruleManager.ActCompleted(entityId, animationKey.animationParams.type);
            }
        }
        if ((EventType)event.index() == EventType::AttackEntity) {
            auto& attackEvent = std::get<AttackEntityEvent>(event);
            auto* attackerCombat = combatComponents.GetObjectPtr(attackEvent.attackerId);
            auto* defenderCombat = combatComponents.GetObjectPtr(attackEvent.defenderId);
            auto& attackerName = *nameComponents.GetObjectPtr(attackEvent.attackerId);
            auto& defenderName = *nameComponents.GetObjectPtr(attackEvent.defenderId);

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

            auto& resourceManager = Core::GetInstance().GetResourceManager();
            StringId attackString = resourceManager.CreateString(buffer);
            Core::GetInstance().PostEvent(GameplayEventLogEvent(attackString));

            if (defenderCombat->GetHealth() <= 0.0f) {
                Core::GetInstance().PostEvent(EntityKilledEvent(
                    attackEvent.attackerId,
                    attackEvent.defenderId));

                sprintf_s(buffer, "%s slew %s!",
                    attackerName.GetName(),
                    defenderName.GetName());
                StringId killString = resourceManager.CreateString(buffer);
                Core::GetInstance().PostEvent(GameplayEventLogEvent(killString));

                auto& transform = *Core::GetInstance().GetEntityManager().GetTransform(
                    attackEvent.defenderId);

                AnimateEntityParameters animationParams;
                animationParams.type = AnimationType::Death;
                animationParams.entityId = attackEvent.defenderId;
                animationParams.startPosition = transform.GetPosition();
                animationParams.endPosition = transform.GetPosition();
                animationParams.startRotation = transform.GetRotation();
                animationParams.endRotation = transform.GetRotation();
                Core::GetInstance().PostEvent(AnimateEntityEvent(std::move(animationParams)));

                Core::GetInstance().PostEvent(DestroyComponentEvent(attackEvent.defenderId, 
                    ComponentType::Combat));
            }
        }
    }

    void GameplayManager::CreateComponent(const ComponentCreationArgs& creationArgs)
    {
        assert(creationArgs.type == ComponentType::PlayerController || 
            creationArgs.type == ComponentType::MonsterController);
        if (creationArgs.type == ComponentType::PlayerController) {
            playerControllerComponents.EmplaceObject(creationArgs.parent,
                PlayerControllerComponent(creationArgs));
            ruleManager.SetPlayer(creationArgs.parent);
        }
        else if (creationArgs.type == ComponentType::MonsterController) {
            monsterControllerComponents.EmplaceObject(creationArgs.parent,
                MonsterControllerComponent(creationArgs));
        }
    }

    void GameplayManager::CreateComponent(const ComponentCreationArgs& creationArgs,
        float health,
        float damage) {
        assert(creationArgs.type == ComponentType::Combat);
        combatComponents.EmplaceObject(creationArgs.parent,
            CombatComponent(creationArgs, health, damage));
    }

    void GameplayManager::CreateComponent(const ComponentCreationArgs& creationArgs,
        const char* name) {
        assert(creationArgs.type == ComponentType::Name);
        nameComponents.EmplaceObject(creationArgs.parent,
            NameComponent(creationArgs, name));
    }

    void GameplayManager::PreDeleteComponent(EntityId id, ComponentType type)
    {
        auto& transform = *Core::GetInstance().GetEntityManager().GetTransform(id);
        vec3 logicalPosition = transform.GetLogicalPosition();
        currentLevel->ClearCreature(logicalPosition);
    }

    void GameplayManager::DeleteComponent(EntityId id, ComponentType type)
    {
        assert(type == ComponentType::PlayerController
            || type == ComponentType::MonsterController
            || type == ComponentType::Combat
            || type == ComponentType::Name);
        if (type == ComponentType::PlayerController) {
            playerControllerComponents.DeleteObject(id);
        }
        else if (type == ComponentType::MonsterController) {
            monsterControllerComponents.DeleteObject(id);
        }
        else if (type == ComponentType::Combat) {
            combatComponents.DeleteObject(id);
        }
        else if (type == ComponentType::Name) {
            nameComponents.DeleteObject(id);
        }
    }
}