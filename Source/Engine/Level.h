#pragma once
#include "Array.h"
#include "Tile.h"
#include "vec3.h"

namespace Plasmium {
    class Level {
    protected:
        Array<EntityId> entities;
        Array<Array<Tile>> map;
        uint32 height = 0;
        uint32 width = 0;

        bool IsPositionInBounds(vec3 logicalPostion);
        Tile& GetTile(vec3 logicalPostion);

    public:
        Level() {}
        virtual ~Level() {}
        virtual void Load() = 0;
        virtual void Unload() = 0;
        bool IsWalkable(vec3 logicalPostion);
        void SetCreature(EntityId entityId, vec3 logicalPostion);
        bool HasCreature(vec3 logicalPostion);
        EntityId GetCreature(vec3 logicalPostion);
        void ClearCreature(vec3 logicalPostion);
    };
}