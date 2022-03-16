#pragma once
#include "Array.h"
#include "Types.h"
#include "vec3.h"

namespace Plasmium {
    struct AnimationKey {
        milliseconds time;
        vec3 rotation;
        vec3 postion;
    };
    struct Animation {
        EntityId entityId;
        Array<AnimationKey> keys;
    };
}