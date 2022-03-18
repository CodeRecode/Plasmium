#pragma once
#include "Types.h"
#include "Model.h"
#include "Texture.h"
#include "InputTypes.h"
#include <variant>
#include "vec3.h"

namespace Plasmium
{

    enum class EventType {
        Input = 0,
        MoveCamera,
        MoveEntity,
        MoveEntityComplete,
        AttackEntity,
        EntityKilled,
        ModelLoaded,
        TextureLoaded,
        PerformanceCounters,
        EntityCreated,
        DestroyEntity,
        GameplayEventLog,
        EventTypeCount
    };

    struct BaseEvent {
        EventType type;
    protected:
        BaseEvent(EventType type) : type(type) { }
    };

    struct InputEvent : BaseEvent {
        const bool (&currentStates)[256];
        const bool (&previousStates)[256];
        float mouseWheelDelta;
        InputEvent(bool (&current)[256], bool(&previous)[256], float mouseWheel) :
            BaseEvent(EventType::Input), 
            currentStates(current),
            previousStates(previous),
            mouseWheelDelta(mouseWheel)
        {}
        bool GetKeyDown(InputKey key) const { return currentStates[(int)key]; }
        bool GetKeyWasDown(InputKey key) const { return previousStates[(int)key]; }
        bool GetKeyToggleDown(InputKey key) const { return currentStates[(int)key] && !previousStates[(int)key]; }
        bool GetKeyToggleUp(InputKey key) const { return !currentStates[(int)key] && previousStates[(int)key]; }
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

    enum class MovementType { Absolute, Relative };
    struct MoveEntityEvent : BaseEvent {
        EntityId entityId;
        vec3 logicalPosition;
        vec3 position;
        MovementType positionType;
        vec3 rotation;
        MovementType rotationType;
        MoveEntityEvent(EntityId entityId,
            vec3 logicalPosition,
            vec3 position,
            MovementType positionType,
            vec3 rotation, 
            MovementType rotationType) :
            BaseEvent(EventType::MoveEntity),
            entityId(entityId),
            logicalPosition(logicalPosition),
            position(position),
            positionType(positionType),
            rotation(rotation),
            rotationType(rotationType)
        {}
    };

    struct MoveEntityCompleteEvent : BaseEvent {
        EntityId entityId;
        vec3 logicalPositionStart;
        vec3 logicalPositionEnd;
        MoveEntityCompleteEvent(EntityId entityId,
            vec3 logicalPositionStart,
            vec3 logicalPositionEnd) :
            BaseEvent(EventType::MoveEntityComplete),
            entityId(entityId),
            logicalPositionStart(logicalPositionStart),
            logicalPositionEnd(logicalPositionEnd)
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

    struct AttackEntityEvent : BaseEvent {
        EntityId attackerId;
        EntityId defenderId;
        vec3 rotation;
        AttackEntityEvent(EntityId attackerId,
            EntityId defenderId,
            vec3 rotation) :
            BaseEvent(EventType::AttackEntity),
            attackerId(attackerId),
            defenderId(defenderId),
            rotation(rotation)
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

    struct DestroyEntityEvent : BaseEvent {
        EntityId entityId;
        DestroyEntityEvent(EntityId entityId) :
            BaseEvent(EventType::DestroyEntity),
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
    typedef std::variant<InputEvent,
        MoveCameraEvent,
        MoveEntityEvent,
        MoveEntityCompleteEvent,
        AttackEntityEvent,
        EntityKilledEvent,
        ModelLoadedEvent,
        TextureLoadedEvent,
        PerformanceCountersEvent,
        EntityCreatedEvent,
        DestroyEntityEvent,
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