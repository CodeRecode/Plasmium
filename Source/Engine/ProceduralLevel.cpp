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

    void ProceduralLevel::Deserialize(std::ifstream& input)
    {
        rapidjson::Document document;
        document.ParseStream(rapidjson::IStreamWrapper(input));

        auto& entityManager = Core::GetEntityManager();

        auto& levelData = document["level_data"];
        SetDimensions(levelData["height"].GetUint(), levelData["width"].GetUint());
        uint32 squareCount = levelData["square_count"].GetUint();

        for (uint32 i = 0; i < squareCount; ++i) {
            PlaceFloors();
        }

        for (uint32 row = 0; row < GetHeight(); ++row) {
            for (uint32 col = 0; col < GetWidth(); ++col) {
                CheckAndSetIsWall(row, col);
            }
        }

        for (uint32 row = 0; row < GetHeight(); ++row) {
            for (uint32 col = 0; col < GetWidth(); ++col) {
                Tile& tile = map[row][col];
                if (tile.GetRoomIndex() != -1 || tile.GetGeometry() != TileGeometry::Floor) {
                    continue;
                }
                FindRoomsFloodFill(row, col, rooms.Size());
                rooms.Push(vec2(row, col));
            }
        }

        auto& tileDefs = document["tile_defs"];
        FileResource floor1ModelFile = FileResource(tileDefs["floor1"].GetString());

        FileResource debugTexture = FileResource("Assets\\Wood032_2K_Color.png");

        for (uint32 row = 0; row < GetHeight(); ++row) {
            for (uint32 col = 0; col < GetWidth(); ++col) {
                Tile& tile = map[row][col];
                if (tile.GetGeometry() == TileGeometry::Floor) {
                    CreateTile(row, col, floor1ModelFile);
                }
                else if (tile.GetGeometry() == TileGeometry::Wall) {
                    CreateTexturedTile(row, col, floor1ModelFile, debugTexture);
                }
            }
        }

        auto& creatureDefs = document["creature_defs"];
        CreateCreature(25, 25, 180.0f, FileResource(creatureDefs["player"].GetString()));
    }

    void ProceduralLevel::PlaceFloors()
    {
        uint32 height = Core::GetNextRandom() % 6 + 3;
        uint32 width = Core::GetNextRandom() % 6 + 3;
        rect absoluteDimensions;
        absoluteDimensions.top = (float)(Core::GetNextRandom() % (GetHeight() - height));
        absoluteDimensions.left = (float)(Core::GetNextRandom() % (GetWidth() - width));

        absoluteDimensions.bottom = absoluteDimensions.top + height;
        absoluteDimensions.right = absoluteDimensions.left + width;

        for (float row = absoluteDimensions.top; row <= absoluteDimensions.bottom; ++row) {
            for (float col = absoluteDimensions.left; col <= absoluteDimensions.right; ++col) {
                map[row][col].SetGeometry(TileGeometry::Floor);
            }
        }
    }

    void ProceduralLevel::FindRoomsFloodFill(uint32 row, uint32 col, uint32 roomIndex)
    {
        if (row < 0 || col < 0 || row >= GetHeight() || col >= GetWidth()) {
            return;
        }

        Tile& tile = map[row][col];
        if (tile.GetRoomIndex() != -1 || tile.GetGeometry() != TileGeometry::Floor) {
            return;
        }

        tile.SetRoomIndex(roomIndex);
        FindRoomsFloodFill(row + 1, col, roomIndex);
        FindRoomsFloodFill(row - 1, col, roomIndex);
        FindRoomsFloodFill(row, col + 1, roomIndex);
        FindRoomsFloodFill(row, col - 1, roomIndex);
    }

    void ProceduralLevel::CheckAndSetIsWall(uint32 row, uint32 col)
    {
        Tile& tile = map[row][col];
        if (tile.GetGeometry() != TileGeometry::Floor) {
            return;
        }

        if (row == 0 || col == 0 || row == GetHeight() - 1 || col == GetWidth() - 1) {
            tile.SetGeometry(TileGeometry::Wall);
            return;
        }

        bool isEdge = map[row + 1][col].GetGeometry() == TileGeometry::None
            || map[row - 1][col].GetGeometry() == TileGeometry::None
            || map[row][col + 1].GetGeometry() == TileGeometry::None
            || map[row][col - 1].GetGeometry() == TileGeometry::None
            || map[row + 1][col + 1].GetGeometry() == TileGeometry::None
            || map[row + 1][col - 1].GetGeometry() == TileGeometry::None
            || map[row - 1][col + 1].GetGeometry() == TileGeometry::None
            || map[row - 1][col - 1].GetGeometry() == TileGeometry::None;

        if (isEdge) {
            tile.SetGeometry(TileGeometry::Wall);
        }
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

    void ProceduralLevel::CreateTexturedTile(float row,
        float col,
        FileResource modelFile,
        FileResource textureFile)
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

        entityManager.AddComponent<ModelComponent>(entityId, modelFile, textureFile);
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