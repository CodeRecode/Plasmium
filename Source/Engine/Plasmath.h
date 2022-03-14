#pragma once

#include "Types.h"
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

    float cosCurr = (float)cos(currentRadians);
    float sinCurr = (float)sin(currentRadians);
    float cosNew = (float)cos(newRadians);
    float sinNew = (float)sin(newRadians);
    // Cross product, probably an easier way
    float direction = (sinCurr * cosNew - cosCurr * sinNew) < 0 ? 1.0f : -1.0f;

    return angle * direction;
}