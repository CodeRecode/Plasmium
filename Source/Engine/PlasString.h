#pragma once
#include "Types.h"
#include "HashFunctions.h"

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

        friend bool operator==(const PlasString& a, const PlasString& b)
        {
            return a.id == b.id;
        }
        friend bool operator!=(const PlasString& a, const PlasString& b)
        {
            return !(a == b);
        }
    };
}

template <>
inline uint32 HashFunc<Plasmium::PlasString>(Plasmium::PlasString value) {
    return value.GetId();
}