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
        void SetWalkable(bool walkable) { this->walkable = walkable; }

        void SetCreature(EntityId creature) {
            this->creature = creature;  
            hasCreature = true; 
        }
        EntityId GetCreature() { return creature; }
        void ClearCreature() { hasCreature = false; }
    };
}