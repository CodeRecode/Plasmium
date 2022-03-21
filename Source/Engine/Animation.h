#pragma once
#include "Array.h"
#include "Types.h"
#include "vec3.h"

namespace Plasmium {

    enum class AnimationType {
        Attack,
        Bump,
        Death,
        Turn,
        Walk,
        AnimationTypeCount
    };

    enum class AnimationKeyType {
        None,
        Attack,
        Bump,
        AnimationComplete,
        AnimationKeyTypeCount
    };

    struct AnimateEntityParameters {
        EntityId entityId;
        EntityId targetId;
        AnimationType type;
        vec3 startPosition;
        vec3 endPosition;
        vec3 startRotation;
        vec3 endRotation;
    };

    struct AnimateEntityKeyEventParameters {
        AnimationKeyType keyType;
        AnimateEntityParameters animationParams;
    };

    struct AnimationKey {
        AnimationKeyType type;
        milliseconds time;
        vec3 position;
        vec3 rotation;
        AnimationKey(AnimationKeyType type, milliseconds time, vec3 position, vec3 rotation) :
            type(type),
            time(time),
            position(position),
            rotation(rotation)
        {}
    };
    struct Animation {
        AnimateEntityParameters params;
        milliseconds startTime;
        milliseconds lastUpdateTime;
        Array<AnimationKey> keys;
    };
}