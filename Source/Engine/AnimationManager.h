#pragma once
#include "Animation.h"
#include "Array.h"
#include "CoreSystem.h"
#include "vec3.h"

namespace Plasmium {
    class AnimationManager : CoreSystem {
    private:
        Array<Animation> animations;
    public:
        virtual void Update(milliseconds deltaTime) override;

        milliseconds CreateAttackAnimation(EntityId entityId, vec3 targetRotation);
        milliseconds CreateBumpAnimation(EntityId entityId, vec3 targetRotation);
        milliseconds CreateDeathAnimation(EntityId entityId);
        milliseconds CreateWalkAnimation(EntityId entityId,
            vec3 endRotation, 
            vec3 endPosition);
    };
}