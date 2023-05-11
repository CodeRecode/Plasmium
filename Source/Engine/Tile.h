#pragma once
#include "Types.h"

namespace Plasmium {
    enum class TileGeometry {
        None, 
        Floor,
        Wall,
        Doorway,
        Hallway
    };

    class Tile {
    private:
        TileGeometry geometry = TileGeometry::None;
        EntityId creature;
        bool hasCreature = false;
        int32 roomIndex = -1;
    public:
        bool IsWalkable() const {
            if (hasCreature) {
                return false;
            }
            return geometry == TileGeometry::Floor || geometry == TileGeometry::Doorway || geometry == TileGeometry::Hallway;
        }

        EntityId HasCreature() const { return hasCreature; }
        EntityId GetCreature() const { return creature; }
        TileGeometry GetGeometry() const { return geometry; }
        int32 GetRoomIndex() const { return roomIndex; }

        void SetCreature(EntityId creature) {
            this->creature = creature;  
            hasCreature = true; 
        }
        void ClearCreature() { hasCreature = false; }
        void SetGeometry(TileGeometry newGeometry) { geometry = newGeometry; }
        void SetRoomIndex(int32 newRoom) { roomIndex = newRoom; }
    };
}