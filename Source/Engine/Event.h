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
        ModelLoaded,
        TextureLoaded,
        PerformanceCounters,
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
        EntityId entity;
        vec3 logicalPosition;
        vec3 position;
        MovementType positionType;
        vec3 rotation;
        MovementType rotationType;
        MoveEntityEvent(EntityId entity,
            vec3 logicalPosition,
            vec3 position,
            MovementType positionType,
            vec3 rotation, 
            MovementType rotationType) :
            BaseEvent(EventType::MoveEntity),
            entity(entity),
            logicalPosition(logicalPosition),
            position(position),
            positionType(positionType),
            rotation(rotation),
            rotationType(rotationType)
        {}
    };


    struct ModelLoadedEvent : BaseEvent {
        FileResource file;
        ModelLoadedEvent(FileResource file) :
            BaseEvent(EventType::ModelLoaded),
            file(file)
        { }
    };

    struct TextureLoadedEvent : BaseEvent {
        FileResource file;
        TextureLoadedEvent(FileResource file) :
            BaseEvent(EventType::TextureLoaded),
            file(file)
        { }
    };

    struct PerformanceCountersEvent : BaseEvent {
        double fps;
        double cpuPercent;
        PerformanceCountersEvent(double fps, double cpuPercent) :
            BaseEvent(EventType::PerformanceCounters),
            fps(fps),
            cpuPercent(cpuPercent)
        { }
    };

    // Order should be sync'd with EventType
    typedef std::variant<InputEvent,
        MoveCameraEvent,
        MoveEntityEvent,
        ModelLoadedEvent,
        TextureLoadedEvent,
        PerformanceCountersEvent> GenericEvent;
}