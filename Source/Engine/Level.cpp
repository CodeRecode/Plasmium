#include "Level.h"
#include "Window.h"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "Core.h"

namespace Plasmium {
    bool Level::IsWalkable(vec3 logicalPostion) {
        uint32 row = (uint32)logicalPostion.z;
        uint32 col = (uint32)logicalPostion.x;
        if (row < 0 || row >= height || col < 0 || col >= width) {
            return false;
        }
        return map[row][col].IsWalkable();
    }
}