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

    public:
        Level() {}
        virtual ~Level() {}
        virtual void Load() = 0;
        virtual void Unload() = 0;
        bool IsWalkable(vec3 logicalPostion);
    };
}