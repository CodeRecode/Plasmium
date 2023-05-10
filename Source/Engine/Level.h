#pragma once
#include "Array.h"
#include "Tile.h"
#include "vec3.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

namespace Plasmium {
    class EntityManager;
    class Level {
    private:
        uint32 height = 0;
        uint32 width = 0;

    protected:
        Array<EntityId> entities;
        Array<Array<Tile>> map;

        void SetDimensions(uint32 height, uint32 width);
        bool IsPositionInBounds(vec3 logicalPostion);
        Tile& GetTile(vec3 logicalPostion);

        vec3 GetVec3FromArray(const rapidjson::GenericArray<false, rapidjson::Value>& arr);
        void AddComponents(EntityManager& entityManager, 
            EntityId entityId,
            rapidjson::Value& jsonValue);

        uint32 GetHeight() { return height; }
        uint32 GetWidth() { return width; }


    public:
        Level() {}
        virtual ~Level() {}
        virtual void Load() = 0;
        virtual void Unload();
        bool IsWalkable(vec3 logicalPostion);
        bool HasWall(vec3 logicalPostion);
        void SetCreature(EntityId entityId, vec3 logicalPostion);
        bool HasCreature(vec3 logicalPostion);
        EntityId GetCreature(vec3 logicalPostion);
        void ClearCreature(vec3 logicalPostion);
    };
}