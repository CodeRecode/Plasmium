#pragma once
#include "Animation.h"
#include "Array.h"
#include "CoreSystem.h"
#include "vec3.h"

namespace Plasmium {
    class AnimationManager : CoreSystem {
    private:
        Array<Animation> animations;

        void CreateAttackAnimation(const AnimateEntityParameters& params);
        void CreateBumpAnimation(const AnimateEntityParameters& params);
        void CreateDeathAnimation(const AnimateEntityParameters& params);
        void CreateWalkAnimation(const AnimateEntityParameters& params);

    public:
        virtual void Update(milliseconds deltaTime) override;
        void CreateAnimation(const AnimateEntityParameters& params);
        void StopAll();
    };
}