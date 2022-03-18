#pragma once

typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

typedef double             milliseconds;
typedef float              degrees;
typedef float              radians;

typedef unsigned int       ComponentId;
typedef unsigned int       EntityId;

typedef unsigned int       FileId;
typedef unsigned int       ResourceId;
typedef unsigned int       StringId;

enum class Direction {
    N = 0,
    NW,
    W,
    SW,
    S,
    SE,
    E,
    NE,
    DirectionCount
};