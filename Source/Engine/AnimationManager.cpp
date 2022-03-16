#include "AnimationManager.h"
#include "Core.h"
#include "Plasmath.h"
#include "Event.h"

namespace Plasmium {

    milliseconds AnimationManager::CreateBumpAnimation(EntityId entityId, vec3 targetRotation)
    {
        milliseconds startTime = Core::GetInstance().GetFrameStartTime();
        auto& transform = *Core::GetInstance()
            .GetEntityManager()
            .GetTransform(entityId);

        assert(!transform.GetAnimating());

        Animation bumpAnimation;
        bumpAnimation.entityId = entityId;

        AnimationKey startKey;
        startKey.time = startTime;
        startKey.postion = transform.GetPosition();
        startKey.rotation = transform.GetRotation();
        bumpAnimation.keys.Push(startKey);

        AnimationKey rotateKey;
        rotateKey.time = startKey.time + 110;
        rotateKey.postion = transform.GetPosition();
        rotateKey.rotation = targetRotation;
        bumpAnimation.keys.Push(rotateKey);

        vec3 bumpDistance = vec3(0.0f, 0.0f, 1.0f);
        mat4 rotationMatrix = mat4(1.0f).Rotate(targetRotation).Transpose();
        vec3 bumpDelta = rotationMatrix * bumpDistance;
        vec3 bumpPosition = transform.GetPosition() + bumpDelta;

        AnimationKey moveForwardKey;
        moveForwardKey.time = bumpAnimation.keys.Back().time + 100;
        moveForwardKey.postion = bumpPosition;
        moveForwardKey.rotation = targetRotation;
        bumpAnimation.keys.Push(moveForwardKey);

        AnimationKey stationaryKey;
        stationaryKey.time = bumpAnimation.keys.Back().time + 50;
        stationaryKey.postion = bumpPosition;
        stationaryKey.rotation = targetRotation;
        bumpAnimation.keys.Push(stationaryKey);

        AnimationKey moveBackwardKey;
        moveBackwardKey.time = bumpAnimation.keys.Back().time + 200;
        moveBackwardKey.postion = transform.GetPosition();
        moveBackwardKey.rotation = targetRotation;
        bumpAnimation.keys.Push(moveBackwardKey);

        animations.Push(bumpAnimation);
        transform.SetAnimating(true);

        return bumpAnimation.keys.Back().time;
    }

    milliseconds AnimationManager::CreateWalkAnimation(EntityId entityId,
        vec3 endPosition,
        vec3 endRotation)
    {
        milliseconds startTime = Core::GetInstance().GetFrameStartTime(); 
        auto& transform = *Core::GetInstance()
            .GetEntityManager()
            .GetTransform(entityId);

        assert(!transform.GetAnimating());

        Animation walkAnimation;
        walkAnimation.entityId = entityId;

        AnimationKey startKey;
        startKey.time = startTime;
        startKey.postion = transform.GetPosition();
        startKey.rotation = transform.GetRotation();
        walkAnimation.keys.Push(startKey);

        if (endRotation != transform.GetRotation()) {
            float angle = FindTurningAngle(transform.GetRotation().y, endRotation.y);
            AnimationKey rotateKey;
            // take ~1ms per degree
            rotateKey.time = walkAnimation.keys.Back().time + fabsf(angle);
            rotateKey.postion = transform.GetPosition();
            rotateKey.rotation = endRotation;
            walkAnimation.keys.Push(rotateKey);
        }

        AnimationKey moveKey;
        moveKey.time = walkAnimation.keys.Back().time + 150;
        moveKey.postion = endPosition;
        moveKey.rotation = endRotation;
        walkAnimation.keys.Push(moveKey);

        animations.Push(walkAnimation);
        transform.SetAnimating(true);

        return walkAnimation.keys.Back().time;
    }

    void AnimationManager::Update(milliseconds deltaTime)
    {
        milliseconds currentTime = Core::GetInstance().GetFrameStartTime();

        for (uint32 animationIndex = 0; animationIndex < animations.Size(); ++animationIndex) {
            auto& animation = animations[animationIndex];
            auto& transform = *Core::GetInstance()
                .GetEntityManager()
                .GetTransform(animation.entityId);

            uint32 keyIndex = 1;
            auto& fromKey = animation.keys[keyIndex - 1];
            auto& toKey = animation.keys[keyIndex];
            ++keyIndex;

            while (keyIndex < animation.keys.Size() && toKey.time < currentTime) {
                fromKey = toKey;
                toKey = animation.keys[keyIndex];
                ++keyIndex;
            }

            if (keyIndex == animation.keys.Size() && currentTime >= toKey.time) {
                transform.SetPosition(toKey.postion);
                transform.SetRotation(toKey.rotation);
                transform.SetAnimating(false);
                animations.Delete(animationIndex);
                --animationIndex;
                continue;
            }

            double timeLerp = (currentTime - fromKey.time) / 
                (toKey.time - fromKey.time);

            vec3 newPosition = (toKey.postion - fromKey.postion)
                * (float)timeLerp 
                + fromKey.postion;

            float angle = FindTurningAngle(fromKey.rotation.y, toKey.rotation.y);
            vec3 newRotation = (vec3(0, angle, 0)) * (float)timeLerp + fromKey.rotation;

            transform.SetPosition(newPosition);
            transform.SetRotation(newRotation);
        }
    }
}