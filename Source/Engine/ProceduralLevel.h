#pragma once
#include "Level.h"
#include "FileResource.h"
#include "rect.h"

#include <fstream>

namespace Plasmium {
    class EntityManager;

    class ProceduralLevel : public Level {
    private:
        FileResource file;
        uint32 seed = 0;

        void Deserialize(std::ifstream& input);
        void CreateRoom(rapidjson::Document& document, rect dimensions);

        void CreateTiles(rapidjson::Value& tileDefs,
            const float RowStart,
            const float Rows, 
            const float ColStart,
            const float Cols);
        void CreateTile(float row,
            float col,
            FileResource modelFile);
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
        void Unload() override;

        FileId GetId() const { return file.GetId(); }
    };
}