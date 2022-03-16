#include "Level.h"
#include "Window.h"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "Core.h"

namespace Plasmium {

    bool Level::IsPositionInBounds(vec3 logicalPostion)
    {
        uint32 row = (uint32)logicalPostion.z;
        uint32 col = (uint32)logicalPostion.x;
        return row >= 0 && row < height&& col >= 0 && col < width;
    }

    Tile& Level::GetTile(vec3 logicalPostion)
    {
        assert(IsPositionInBounds(logicalPostion));
        uint32 row = (uint32)logicalPostion.z;
        uint32 col = (uint32)logicalPostion.x;
        return map[row][col];
    }

    bool Level::IsWalkable(vec3 logicalPostion)
    {
       return IsPositionInBounds(logicalPostion) && GetTile(logicalPostion).IsWalkable();
    }

    void Level::SetCreature(EntityId entityId, vec3 logicalPostion)
    {
        return GetTile(logicalPostion).SetCreature(entityId);
    }

    void Level::ClearCreature(vec3 logicalPostion)
    {
        return GetTile(logicalPostion).ClearCreature();
    }
}