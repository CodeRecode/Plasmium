#pragma once
#include "Types.h"

template <typename T>
uint32 HashFunc(T value) {
    assert(false && "HashFunc not implemented");
    return 0;
}

template <>
inline uint32 HashFunc<uint32>(uint32 value) {
    return value;
}

// https://stackoverflow.com/questions/8317508/hash-function-for-a-string
template <>
inline uint32 HashFunc<const char*>(const char* value) {
    uint32 hash = 37, index = 0;
    while (value[index] != 0) {
        hash = (hash * 54059) ^ (value[index] * 76963);
        ++index;
    }
    return hash;
}