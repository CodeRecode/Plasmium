#pragma once

#include "Types.h"

namespace Plasmium
{
    class rect {
    private:
    public:
        union {
            struct { float x, y, width, height; };
            struct { float left, top, right, bottom; };
        };
        rect() : x(0), y(0), width(0), height(0) {}
        rect(float x, float y, float width, float height) :
            x(x),
            y(y),
            width(width),
            height(height)
        {}
    };

    bool operator==(const rect& lhs, const rect& rhs);
    bool operator!=(const rect& lhs, const rect& rhs);
}
