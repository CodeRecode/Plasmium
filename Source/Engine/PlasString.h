#pragma once
#include "Types.h"

namespace Plasmium {
    class PlasString {
    private:
        char string[256];
        uint32 length;
        StringId id;
    public:
        PlasString(const char* string);

        const char* Get() const { return string; }
        const StringId GetId() const { return id; }
        uint32 Size() const { return length; }
    };
}