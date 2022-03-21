#pragma once
#include "Animation.h"
#include "Component.h"
#include "Model.h"
#include "Keybinds.h"
#include "Texture.h"
#include "Types.h"
#include "vec3.h"

#include <variant>

namespace Plasmium
{

    enum class EventType {
        Input = 0,
        MoveCamera,
        TryMoveEntity,
        AnimateEntity,
        AnimateEntityKey,
        ChangeTransform,
        AttackEntity,
        EntityKilled,
        ModelLoaded,
        TextureLoaded,
        PerformanceCounters,
        EntityCreated,
        DestroyComponent,
        DestroyEntity,
        PassTurn,
        GameplayEventLog,
        EventTypeCount
    };

    struct BaseEvent {
        EventType type;
    protected:
        BaseEvent(EventType type) : type(type) { }
    };

    struct InputEvent : BaseEvent {
        const bool (&currentStates)[(uint32)KeybindFunction::KeybindFunctionCount];
        const bool (&previousStates)[(uint32)KeybindFunction::KeybindFunctionCount];
        InputEvent(bool (&current)[(uint32)KeybindFunction::KeybindFunctionCount],
            bool(&previous)[(uint32)KeybindFunction::KeybindFunctionCount]) :
            BaseEvent(EventType::Input), 
            currentStates(current),
            previousStates(previous)
        {}
        bool GetKeyDown(KeybindFunction key) const { return currentStates[(int)key]; }
        bool GetKeyWasDown(KeybindFunction key) const { return previousStates[(int)key]; }
        bool GetKeyToggleDown(KeybindFunction key) const { return currentStates[(int)key] && !previousStates[(int)key]; }
        bool GetKeyToggleUp(KeybindFunction key) const { return !currentStates[(int)key] && previousStates[(int)key]; }
    };

    struct MoveCameraEvent : BaseEvent {
        vec3 position;
        vec3 rotation;
        MoveCameraEvent(vec3 position, vec3 rotation) :
            BaseEvent(EventType::MoveCamera),
            position(position),
            rotation(rotation)
        {}
    };

    struct TryMoveEntityEvent : BaseEvent {
        EntityId entityId;
        vec3 relativeLogicalPosition;
        vec3 rotation;
        TryMoveEntityEvent(EntityId entityId,
            vec3 relativeLogicalPosition,
            vec3 rotation) :
            BaseEvent(EventType::TryMoveEntity),
            entityId(entityId),
            relativeLogicalPosition(relativeLogicalPosition),
            rotation(rotation)
        {}
    };

    struct AnimateEntityEvent : BaseEvent {
        AnimateEntityParameters params;
        AnimateEntityEvent(AnimateEntityParameters params) :
            BaseEvent(EventType::AnimateEntity),
            params(params)
        {}
    };
        
    struct AnimateEntityKeyEvent : BaseEvent{
        AnimateEntityKeyEventParameters params;
        AnimateEntityKeyEvent(AnimateEntityKeyEventParameters params) :
            BaseEvent(EventType::AnimateEntityKey),
            params(params)
        {}
    };

    enum ChangeTransformValues {
        ChangeTransformLogicalPosition = 0x01,
        ChangeTransformPosition = 0x02,
        ChangeTransformRotation = 0x04,
        ChangeTransformScale = 0x08,
    };
    struct ChangeTransformEvent : BaseEvent {
        EntityId entityId;
        uint32 changeValues;
        vec3 logicalPosition;
        vec3 position;
        vec3 rotation;
        vec3 scale;
        ChangeTransformEvent(EntityId entityId,
        uint32 changeValues,
        vec3 logicalPosition,
        vec3 position,
        vec3 rotation,
        vec3 scale) :
            BaseEvent(EventType::ChangeTransform),
            entityId(entityId),
            changeValues(changeValues),
            logicalPosition(logicalPosition),
            position(position),
            rotation(rotation),
            scale(scale)
        {}
    };

    struct AttackEntityEvent : BaseEvent {
        EntityId attackerId;
        EntityId defenderId;
        AttackEntityEvent(EntityId attackerId,
            EntityId defenderId) :
            BaseEvent(EventType::AttackEntity),
            attackerId(attackerId),
            defenderId(defenderId)
        {}
    };

    struct EntityKilledEvent : BaseEvent {
        EntityId killerId;
        EntityId dyingId;
        EntityKilledEvent(EntityId killerId,
            EntityId dyingId) :
            BaseEvent(EventType::EntityKilled),
            killerId(killerId),
            dyingId(dyingId)
        {}
    };

    struct ModelLoadedEvent : BaseEvent {
        FileResource file;
        ModelLoadedEvent(FileResource file) :
            BaseEvent(EventType::ModelLoaded),
            file(file)
        {}
    };

    struct TextureLoadedEvent : BaseEvent {
        FileResource file;
        TextureLoadedEvent(FileResource file) :
            BaseEvent(EventType::TextureLoaded),
            file(file)
        {}
    };

    struct PerformanceCountersEvent : BaseEvent {
        double fps;
        double cpuPercent;
        PerformanceCountersEvent(double fps, double cpuPercent) :
            BaseEvent(EventType::PerformanceCounters),
            fps(fps),
            cpuPercent(cpuPercent)
        {}
    }; 

    struct EntityCreatedEvent : BaseEvent {
        EntityId entityId;
        vec3 logicalPosition;
        EntityCreatedEvent(EntityId entityId, vec3 logicalPosition) :
            BaseEvent(EventType::EntityCreated),
            entityId(entityId),
            logicalPosition(logicalPosition)
        {}
    };

    struct DestroyComponentEvent : BaseEvent {
        EntityId entityId;
        ComponentType type;
        DestroyComponentEvent(EntityId entityId, ComponentType type) :
            BaseEvent(EventType::DestroyComponent),
            entityId(entityId),
            type(type)
        {}
    };

    struct DestroyEntityEvent : BaseEvent {
        EntityId entityId;
        DestroyEntityEvent(EntityId entityId) :
            BaseEvent(EventType::DestroyEntity),
            entityId(entityId)
        {}
    };

    struct PassTurnEvent : BaseEvent {
        EntityId entityId;
        PassTurnEvent(EntityId entityId) :
            BaseEvent(EventType::PassTurn),
            entityId(entityId)
        {}
    };

    struct GameplayEventLogEvent : BaseEvent {
        StringId stringId;
        GameplayEventLogEvent(StringId stringId) :
            BaseEvent(EventType::GameplayEventLog),
            stringId(stringId)
        {}
    };

    // Order should be sync'd with EventType
    typedef std::variant < InputEvent,
        MoveCameraEvent,
        TryMoveEntityEvent,
        AnimateEntityEvent,
        AnimateEntityKeyEvent,
        ChangeTransformEvent,
        AttackEntityEvent,
        EntityKilledEvent,
        ModelLoadedEvent,
        TextureLoadedEvent,
        PerformanceCountersEvent,
        EntityCreatedEvent,
        DestroyComponentEvent,
        DestroyEntityEvent,
        PassTurnEvent,
        GameplayEventLogEvent> GenericEvent;

    struct DeferredEvent {
        GenericEvent event;
        milliseconds eventTime;
        DeferredEvent(GenericEvent event, milliseconds eventTime) :
            event(event),
            eventTime(eventTime)
        {}
    };
}