#pragma once
#include "Level.h"
#include "FileResource.h"
#include "rect.h"
#include "vec2.h"

#include <fstream>

namespace Plasmium {
    class EntityManager;

    class ProceduralLevel : public Level {
    private:
        FileResource file;
        uint32 seed = 0;
        Array<vec2> rooms;

        void Deserialize(std::ifstream& input);
        void PlaceFloors();
        void FindRoomsFloodFill(uint32 row, uint32 col, uint32 roomIndex);
        void CheckAndSetIsWall(uint32 row, uint32 col);

        void CreateRoom(rapidjson::Document& document, rect dimensions);
        void CreateTiles(rapidjson::Value& tileDefs,
            const float RowStart,
            const float Rows, 
            const float ColStart,
            const float Cols);
        void CreateTile(float row,
            float col,
            FileResource modelFile);
        void CreateTexturedTile(float row,
            float col,
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