#pragma once

#include "Types.h"

namespace Plasmium
{
    template <typename T = float>
    class rect {
    private:
    public:
        union {
            struct { T x, y, width, height; };
            struct { T left, top, right, bottom; };
        };
        rect() : x(0), y(0), width(0), height(0) {}
        rect(T x, T y, T width, T height) :
            x(x),
            y(y),
            width(width),
            height(height)
        {}

        friend bool operator==(const rect&lhs, const rect&rhs)
        {
            return lhs.x == rhs.x &&
                lhs.y == rhs.y &&
                lhs.width == rhs.width &&
                lhs.height == rhs.height;
        }

        friend bool operator!=(const rect& lhs, const rect& rhs)
        {
            return !(lhs == rhs);
        }
    };
}
