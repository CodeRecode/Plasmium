#include "rect.h"

namespace Plasmium
{
    bool operator==(const rect& lhs, const rect& rhs)
    {
        return lhs.x == rhs.x &&
               lhs.y == rhs.y &&
               lhs.width == rhs.width &&
               lhs.height == rhs.height;
    }

    bool operator!=(const rect& lhs, const rect& rhs)
    {
        return !(lhs == rhs);
    }
}