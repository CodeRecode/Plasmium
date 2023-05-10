#pragma once

#include "Types.h"
#include "vec3.h"

#define _USE_MATH_DEFINES
#include <math.h>

inline radians DegreesToRadians(degrees angle)
{
    return angle * (float)M_PI / 180.0f;
}

inline degrees RadiansToDegrees(radians angle)
{
    return angle * 180.0f / (float)M_PI;
}

inline degrees FindTurningAngle(degrees currentFacing, degrees newFacing)
{
    radians currentRadians = DegreesToRadians(currentFacing);
    radians newRadians = DegreesToRadians(newFacing);

    degrees angle = fabsf(currentFacing - newFacing);
    angle = fminf(angle, 360.0f - angle);

    float cosCurr = cosf(currentRadians);
    float sinCurr = sinf(currentRadians);
    float cosNew = cosf(newRadians);
    float sinNew = sinf(newRadians);
    // Cross product, probably an easier way
    float direction = (sinCurr * cosNew - cosCurr * sinNew) < 0 ? 1.0f : -1.0f;

    return angle * direction;
}

template <typename T>
inline T Clamp(T val, T min, T max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}



namespace Plasmium {
    inline vec3 DirectionToRotation(Direction direction)
    {
        return vec3(0.0f, (int32)direction * 45.0f, 0.0f);
    }

    inline vec3 DirectionToVector(Direction direction)
    {
        vec3 result = vec3();
        if (direction == Direction::N || direction == Direction::NE || direction == Direction::NW) {
            result.z = -1;
        }
        else if (direction == Direction::S || direction == Direction::SE || direction == Direction::SW) {
            result.z = 1;
        }

        if (direction == Direction::E || direction == Direction::NE || direction == Direction::SE) {
            result.x = 1;
        }
        else if (direction == Direction::W || direction == Direction::NW || direction == Direction::SW) {
            result.x = -1;
        }
        return result;
    }

    inline vec3 VectorToRotation(vec3 vector)
    {
        return vec3(0.0f, -RadiansToDegrees(atan2f(vector.x, -vector.z)), 0.0f);
    }
}