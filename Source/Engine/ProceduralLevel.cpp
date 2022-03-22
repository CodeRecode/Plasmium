#include "ProceduralLevel.h"

#include "AllComponents.h"
#include "Core.h"
#include "EntityManager.h"
#include "vec2.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

namespace Plasmium {

    void ProceduralLevel::Load()
    {
        if (seed == 0) {
            // Cache the seed for reload
            seed = Core::GetNextRandom();
        }
        Core::SetSeed(seed);
        Deserialize(file.GetInputStream());
    }

    void ProceduralLevel::Unload()
    {
        entities.Clear();
        map.Clear();
    }

    void ProceduralLevel::Deserialize(std::ifstream& input)
    {
        rapidjson::Document document;
        document.ParseStream(rapidjson::IStreamWrapper(input));

        auto& entityManager = Core::GetEntityManager();

        auto& roomDefs = document["room_defs"].GetArray();
        auto& room1 = roomDefs[0].GetArray();
        vec2 roomDims = vec2(room1[0].GetFloat(), room1[1].GetFloat());
        SetDimensions((uint32)roomDims.x, (uint32)roomDims.y);

        CreateRoom(document, rect(10, 0, 8, 8));
        CreateRoom(document, rect(0, 0, 5, 5));

        auto& creatureDefs = document["creature_defs"];
        CreateCreature(13, 3, 180.0f, FileResource(creatureDefs["player"].GetString()));
    }

    void ProceduralLevel::CreateRoom(rapidjson::Document& document, rect dimensions)
    {
        auto& tileDefs = document["tile_defs"];
        CreateTiles(tileDefs, dimensions.x, dimensions.height, dimensions.y, dimensions.width);

        auto& wallDefs = document["wall_defs"];
        CreateWalls(wallDefs, dimensions.x, dimensions.height, dimensions.y, dimensions.width);
    }

    void ProceduralLevel::CreateTiles(rapidjson::Value& tileDefs,
        const float RowStart,
        const float Rows,
        const float ColStart,
        const float Cols)
    {
        FileResource floor1ModelFile = FileResource(tileDefs["floor1"].GetString());
        FileResource floor2ModelFile = FileResource(tileDefs["floor2"].GetString());
        FileResource floor3ModelFile = FileResource(tileDefs["floor3"].GetString());

        // Fill with 3x3s
        for (float row = RowStart; row + 3 <= RowStart + Rows; row += 3) {
            for (float col = ColStart; col + 3 <= ColStart + Cols; col += 3) {
                CreateTile(row + 1.0f, col + 1.0f, floor3ModelFile);
            }
        }
        // Fill last column
        uint32 remainingColTiles = (uint32)Cols % 3;
        if (remainingColTiles > 0) {
            float row = RowStart;
            for (; row + remainingColTiles <= RowStart + Rows; row += remainingColTiles) {
                if (remainingColTiles == 2) {
                    CreateTile(row + 0.5f, ColStart + Cols - 1.5f, floor2ModelFile);
                }
                else {
                    CreateTile(row, ColStart + Cols - 1.0f, floor1ModelFile);
                }
            }
            // Last 2x2 couldn't fit, use 2 1x1s
            if (row < Rows) {
                CreateTile(row, ColStart + Cols - 1.0f, floor1ModelFile);
                CreateTile(row, ColStart + Cols - 2.0f, floor1ModelFile);
            }
        }

        // Fill last row
        uint32 remainingRowTiles = (uint32)Rows % 3;
        if (remainingRowTiles > 0) {
            float col = ColStart;
            for (; col + remainingRowTiles <= ColStart + Cols - remainingColTiles; col += remainingRowTiles) {
                if (remainingRowTiles == 2) {
                    CreateTile(RowStart + Rows - 1.5f, col + 0.5f, floor2ModelFile);
                }
                else {
                    CreateTile(RowStart + Rows - 1.0f, col, floor1ModelFile);
                }
            }
            // Last 2x2 couldn't fit, use 2 1x1s
            if (col < Cols) {
                CreateTile(RowStart + Rows - 1.0f, col, floor1ModelFile);
                CreateTile(RowStart + Rows - 2.0f, col, floor1ModelFile);
            }
        }
    }

    void ProceduralLevel::CreateTile(float row, 
        float col, 
        FileResource modelFile)
    {
        auto& entityManager = Core::GetEntityManager();
        EntityId entityId = entityManager.CreateEntity();
        entities.Push(entityId);
        vec3 tilePosition = vec3(col, -0.5f, row);

        entityManager.AddComponent<TransformComponent>(entityId,
            vec3(),
            vec3(TransformComponent::LogicalPointToWorld(tilePosition)),
            vec3(),
            vec3(1.0f));

        entityManager.AddComponent<ModelComponent>(entityId, modelFile);
    }

    void ProceduralLevel::CreateWalls(rapidjson::Value& wallDefs,
        const float RowStart,
        const float Rows,
        const float ColStart,
        const float Cols)
    {
        FileResource wallModelFile = FileResource(wallDefs["wall"].GetString());
        FileResource cornerModelFile = FileResource(wallDefs["corner"].GetString());
        FileResource wallEndModelFile = FileResource(wallDefs["wall_end"].GetString());
        FileResource doorModelFile = FileResource(wallDefs["door"].GetString());

        CreateWall(RowStart, ColStart, 180, cornerModelFile);
        CreateWall(RowStart, ColStart + Cols - 1.0f, 90, cornerModelFile);
        CreateWall(RowStart + Rows - 1.0f, ColStart + Cols - 1.0f, 0, cornerModelFile);
        CreateWall(RowStart + Rows - 1.0f, ColStart, -90, cornerModelFile);

        for (float row = RowStart + 2; row < RowStart + Rows - 1; row += 2) {
            CreateWall(row, ColStart, 90, wallModelFile);
            CreateWall(row, ColStart + Cols - 1.0f, 90, wallModelFile);
        }

        for (float col = ColStart + 2; col < ColStart + Cols - 1; col += 2) {
            CreateWall(RowStart, col, 0, wallModelFile);
            CreateWall(RowStart + Rows - 1.0f, col, 0, wallModelFile);
        }
    }

    void ProceduralLevel::CreateWall(float row, 
        float col,
        float rotation,
        FileResource modelFile)
    {
        auto& entityManager = Core::GetEntityManager();
        EntityId entityId = entityManager.CreateEntity();
        entities.Push(entityId);
        vec3 wallPosition = vec3(col, 0.0f, row);

        entityManager.AddComponent<TransformComponent>(entityId,
            vec3(),
            vec3(TransformComponent::LogicalPointToWorld(wallPosition)),
            vec3(0.0f, rotation, 0.0f),
            vec3(1.0f));

        entityManager.AddComponent<ModelComponent>(entityId, modelFile);
    }


    void ProceduralLevel::CreateCreature(float row,
        float col,
        float rotation,
        FileResource archetype)
    {
        auto& entityManager = Core::GetEntityManager();
        EntityId entityId = entityManager.CreateEntity();
        entities.Push(entityId);
        vec3 logicalPosition = vec3(col, 0, row);

        entityManager.AddComponent<TransformComponent>(entityId,
            vec3(logicalPosition),
            vec3(TransformComponent::LogicalPointToWorld(logicalPosition)),
            vec3(0.0f, rotation, 0.0f),
            vec3(1.0f));

        rapidjson::Document archetypeDocument;
        archetypeDocument.ParseStream(rapidjson::IStreamWrapper(archetype.GetInputStream()));
        AddComponents(entityManager, entityId, archetypeDocument);

        Core::PostEvent(EntityCreatedEvent(entityId, logicalPosition));
    }
}