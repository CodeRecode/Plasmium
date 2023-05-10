#pragma once
#include "Level.h"
#include "FileResource.h"
#include "rect.h"
#include "vec2.h"

#include <fstream>

namespace Plasmium {
    class EntityManager;

    struct ProceduralRoom {
        vec2 center;
        rect<uint32> dimensions;

        void SetCenter() {
            center.x = (dimensions.bottom - dimensions.top) / 2.0f + dimensions.top;
            center.y = (dimensions.right - dimensions.left) / 2.0f + dimensions.left;
        }
    };

    class ProceduralLevel : public Level {
    private:
        FileResource file;
        uint32 seed = 0;
        Array<ProceduralRoom> rooms;

        void Deserialize(std::ifstream& input);
        void PlaceFloors();
        void FindRoomsFloodFill(uint32 row, uint32 col, uint32 roomIndex);
        void CheckAndSetIsWall(uint32 row, uint32 col);
        void CreateConnector(ProceduralRoom& start, ProceduralRoom& end);

        void CreateTile(uint32 row,
            uint32 col,
            FileResource modelFile);
        void CreateTexturedTile(uint32 row,
            uint32 col,
            FileResource modelFile,
            FileResource textureFile);
        void CreateWalls(rapidjson::Value& wallDefs,
            const float RowStart,
            const float Rows,
            const float ColStart,
            const float Cols);
        void CreateWall(float row,
            float col,
            float rotation,
            FileResource modelFile);
        void CreateCreature(float row,
            float col,
            float rotation,
            FileResource archetype);

    public:
        ProceduralLevel(FileResource file) : file(file) {}
        void Load() override;

        FileId GetId() const { return file.GetId(); }
    };
}