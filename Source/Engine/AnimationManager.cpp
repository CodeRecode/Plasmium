#include "AnimationManager.h"
#include "Core.h"
#include "Plasmath.h"
#include "Event.h"

namespace Plasmium {


    void AnimationManager::CreateAnimation(const AnimateEntityParameters& params)
    {
        switch (params.type) {
        case AnimationType::Attack:
            CreateAttackAnimation(params);
            break;
        case AnimationType::Bump:
            CreateBumpAnimation(params);
            break;
        case AnimationType::Death:
            CreateDeathAnimation(params);
            break;
        case AnimationType::Walk:
            CreateWalkAnimation(params);
            break;
        default:
            assert(false && "Unknown animation type: " && params.type == AnimationType::AnimationTypeCount);
        }
    }

    void AnimationManager::CreateAttackAnimation(const AnimateEntityParameters& params)
    {
        milliseconds startTime = Core::GetInstance().GetFrameStartTime();

        Animation attackAnimation;
        attackAnimation.params = params;
        attackAnimation.lastUpdateTime = startTime;

        // Start
        attackAnimation.keys.Push(AnimationKey(AnimationKeyType::None, 
            startTime,
            params.startPosition,
            params.startRotation));

        // Rotate
        attackAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            attackAnimation.keys.Back().time + 110,
            params.startPosition,
            params.endRotation));

        vec3 attackDistance = vec3(0.0f, 0.0f, 0.8f);
        mat4 rotationMatrix = mat4(1.0f).Rotate(params.endRotation * -1.0);
        vec3 attackDelta = rotationMatrix * attackDistance;
        vec3 attackPosition = params.startPosition + attackDelta;

        // Move Forward
        attackAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            attackAnimation.keys.Back().time + 50,
            attackPosition,
            params.endRotation));

        // Attack
        attackAnimation.keys.Push(AnimationKey(AnimationKeyType::Attack,
            attackAnimation.keys.Back().time + 100,
            attackPosition,
            params.endRotation));

        // Return
        attackAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            attackAnimation.keys.Back().time + 250,
            params.startPosition,
            params.endRotation));

        animations.Push(attackAnimation);
    }

    void AnimationManager::CreateBumpAnimation(const AnimateEntityParameters& params)
    {
        milliseconds startTime = Core::GetInstance().GetFrameStartTime();
        Animation bumpAnimation;
        bumpAnimation.params = params;
        bumpAnimation.lastUpdateTime = startTime;

        // Start
        bumpAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            startTime,
            params.startPosition,
            params.startRotation));

        // Rotate
        bumpAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            bumpAnimation.keys.Back().time + 110,
            params.startPosition,
            params.endRotation));

        vec3 bumpDistance = vec3(0.0f, 0.0f, 1.0f);
        mat4 rotationMatrix = mat4(1.0f).Rotate(params.endRotation * -1.0);
        vec3 bumpDelta = rotationMatrix * bumpDistance;
        vec3 bumpPosition = params.startPosition + bumpDelta;

        // Move Forward
        bumpAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            bumpAnimation.keys.Back().time + 100,
            bumpPosition,
            params.endRotation));

        // Bump
        bumpAnimation.keys.Push(AnimationKey(AnimationKeyType::Bump,
            bumpAnimation.keys.Back().time + 50,
            bumpPosition,
            params.endRotation));

        // Return
        bumpAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            bumpAnimation.keys.Back().time + 200,
            params.startPosition,
            params.endRotation));

        animations.Push(bumpAnimation);
    }

    void AnimationManager::CreateDeathAnimation(const AnimateEntityParameters& params)
    {
        milliseconds startTime = Core::GetInstance().GetFrameStartTime();
        Animation deathAnimation;
        deathAnimation.params = params;
        deathAnimation.lastUpdateTime = startTime;

        vec3 up = vec3(0.0f, 1.0f, 0.0f);
        vec3 rotationAxis = vec3(90.0f, 0.0f, 0.0f);

        vec3 endPosition = params.startPosition + vec3(0.0f, 0.5f, 0.0f);
        vec3 endRotation = params.startRotation + rotationAxis;

        // Start
        deathAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            startTime,
            params.startPosition,
            params.startRotation));

        // Collapse
        deathAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            deathAnimation.keys.Back().time + 150,
            endPosition,
            endRotation));

        // Despawn
        deathAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            deathAnimation.keys.Back().time + 500,
            endPosition,
            endRotation));

        animations.Push(deathAnimation);
    }

    void AnimationManager::CreateWalkAnimation(const AnimateEntityParameters& params)
    {
        milliseconds startTime = Core::GetInstance().GetFrameStartTime();
        Animation walkAnimation;
        walkAnimation.params = params;
        walkAnimation.lastUpdateTime = startTime;

        // Start
        walkAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            startTime,
            params.startPosition,
            params.startRotation));


        if (params.endRotation != params.startRotation) {
            float angle = FindTurningAngle(params.startRotation.y, params.endRotation.y);
            // take ~1ms per degree
            walkAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
                walkAnimation.keys.Back().time + fabsf(angle),
                params.startPosition,
                params.endRotation));
        }


        // Move Forward
        walkAnimation.keys.Push(AnimationKey(AnimationKeyType::None,
            walkAnimation.keys.Back().time + 150,
            params.endPosition,
            params.endRotation));

        animations.Push(walkAnimation);
    }

    void AnimationManager::Update(milliseconds deltaTime)
    {
        milliseconds currentTime = Core::GetInstance().GetFrameStartTime();

        for (uint32 animationIndex = 0; animationIndex < animations.Size(); ++animationIndex) {
            auto& animation = animations[animationIndex];

            uint32 keyIndex = 1;
            auto& fromKey = animation.keys[keyIndex - 1];
            auto& toKey = animation.keys[keyIndex];
            ++keyIndex;

            while (keyIndex < animation.keys.Size() && toKey.time < currentTime) {
                fromKey = toKey;
                toKey = animation.keys[keyIndex];
                ++keyIndex;
            }

            // Fire the key event if we've just passed it
            if (fromKey.time > animation.lastUpdateTime
                && fromKey.time <= currentTime
                && fromKey.type != AnimationKeyType::None) {
                AnimateEntityKeyEventParameters keyParameters;
                keyParameters.keyType = fromKey.type;
                keyParameters.animationParams = animation.params;
                Core::GetInstance().PostEvent(AnimateEntityKeyEvent(std::move(keyParameters)));
            }

            if (keyIndex == animation.keys.Size() && currentTime >= toKey.time) {
                Core::GetInstance().PostEvent(ChangeTransformEvent(
                    animation.params.entityId,
                    ChangeTransformPosition | ChangeTransformRotation,
                    vec3(),
                    toKey.position,
                    toKey.rotation,
                    vec3()));

                AnimateEntityKeyEventParameters keyParameters;
                keyParameters.keyType = AnimationKeyType::AnimationComplete;
                keyParameters.animationParams = animation.params;
                Core::GetInstance().PostEvent(AnimateEntityKeyEvent(std::move(keyParameters)));

                animations.Delete(animationIndex);
                --animationIndex;
                continue;
            }

            double timeLerp = (currentTime - fromKey.time) / 
                (toKey.time - fromKey.time);

            vec3 newPosition = (toKey.position - fromKey.position)
                * (float)timeLerp 
                + fromKey.position;

            float xAngle = FindTurningAngle(fromKey.rotation.x, toKey.rotation.x);
            float yAngle = FindTurningAngle(fromKey.rotation.y, toKey.rotation.y);
            float zAngle = FindTurningAngle(fromKey.rotation.z, toKey.rotation.z);
            vec3 newRotation = (vec3(xAngle, yAngle, zAngle)) * (float)timeLerp + fromKey.rotation;

            Core::GetInstance().PostEvent(ChangeTransformEvent(
                animation.params.entityId,
                ChangeTransformPosition | ChangeTransformRotation,
                vec3(),
                newPosition,
                newRotation,
                vec3()));

            animation.lastUpdateTime = currentTime;
        }
    }

    void AnimationManager::StopAll()
    {
        animations.Clear();
    }
}