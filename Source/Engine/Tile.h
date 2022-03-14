#pragma once
#include "Types.h"

namespace Plasmium {
    class Tile {
    private:
        EntityId creature;
        bool hasCreature = false;
        bool walkable = true;
    public:
        bool IsWalkable() const { return walkable && !hasCreature; }
        void SetCollision() { walkable = false; }
    };
}