#include "ProceduralLevel.h"

#include "AllComponents.h"
#include "Core.h"
#include "EntityManager.h"
#include "Plasmath.h"
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
                ProceduralRoom room;
                room.dimensions.left = col;
                room.dimensions.right = col;
                room.dimensions.top = row;
                room.dimensions.bottom = row;

                rooms.Push(room);
                FindRoomsFloodFill(row, col, rooms.Size() - 1);
                rooms[rooms.Size() - 1].SetCenter();
            }
        }

        Array<std::pair<float, ProceduralRoom>> roomsByDistance;
        vec2 lastCenter = vec2();
        for (auto& room : rooms) {
            roomsByDistance.Push(std::make_pair(lastCenter.DistanceSquared(room.center), room));
        }
        std::qsort(roomsByDistance.begin(), 
            roomsByDistance.Size(), 
            sizeof(std::pair<float, ProceduralRoom>), 
            [](const void* a, const void* b) {
                const auto pair1 = *(std::pair<float, ProceduralRoom>*)(a);
                const auto pair2 = *(std::pair<float, ProceduralRoom>*)(b);
                return (int32)(pair2.first - pair1.first); // Reverse order
            });

        while (roomsByDistance.Size() > 1) {
            CreateConnector(roomsByDistance[roomsByDistance.Size() - 1].second, 
                roomsByDistance[roomsByDistance.Size() - 2].second);
            if (roomsByDistance.Size() > 2) {
                CreateConnector(roomsByDistance[roomsByDistance.Size() - 1].second,
                    roomsByDistance[roomsByDistance.Size() - 3].second);
            }

            roomsByDistance.Pop();
            lastCenter = roomsByDistance[roomsByDistance.Size() - 1].second.center;
            for (auto& pair : roomsByDistance) {
                pair.first = lastCenter.DistanceSquared(pair.second.center);
            }
            std::qsort(roomsByDistance.begin(),
                roomsByDistance.Size(),
                sizeof(std::pair<float, ProceduralRoom>),
                [](const void* a, const void* b) {
                const auto pair1 = *(std::pair<float, ProceduralRoom>*)(a);
                const auto pair2 = *(std::pair<float, ProceduralRoom>*)(b);
                return (int32)(pair2.first - pair1.first); // Reverse order
            });
        }

        auto& tileDefs = document["tile_defs"];
        FileResource floor1ModelFile = FileResource(tileDefs["floor1"].GetString());

        FileResource debugTexture = FileResource("Assets\\Wood032_2K_Color.png");
        FileResource debugTexture2 = FileResource("Assets\\Metal013_2K_Color.png");
        FileResource debugTexture3 = FileResource("Assets\\Concrete015_4K_Color.jpg");

        for (uint32 row = 0; row < GetHeight(); ++row) {
            for (uint32 col = 0; col < GetWidth(); ++col) {
                Tile& tile = map[row][col];
                if (tile.GetGeometry() == TileGeometry::Floor) {
                    CreateTile(row, col, floor1ModelFile);
                }
                else if (tile.GetGeometry() == TileGeometry::Wall) {
                    CreateTexturedTile(row, col, floor1ModelFile, debugTexture);
                }
                else if (tile.GetGeometry() == TileGeometry::Hallway) {
                    CreateTexturedTile(row, col, floor1ModelFile, debugTexture2);
                }
                else if (tile.GetGeometry() == TileGeometry::Doorway) {
                    CreateTexturedTile(row, col, floor1ModelFile, debugTexture3);
                }
            }
        }

        // Place the player on a walkable tile
        auto& creatureDefs = document["creature_defs"];
        uint32 row = Core::GetNextRandom() % GetHeight();
        uint32 col = Core::GetNextRandom() % GetWidth();
        while (!map[row][col].IsWalkable()) {
            row = Core::GetNextRandom() % GetHeight();
            col = Core::GetNextRandom() % GetWidth();
        }
        CreateCreature(row, col, 180.0f, FileResource(creatureDefs["player"].GetString()));
    }

    void ProceduralLevel::PlaceFloors()
    {
        uint32 height = Core::GetNextRandom() % 5 + 4;
        uint32 width = Core::GetNextRandom() % 5 + 4;
        rect<uint32> absoluteDimensions;
        absoluteDimensions.top = Core::GetNextRandom() % (GetHeight() - height);
        absoluteDimensions.left = Core::GetNextRandom() % (GetWidth() - width);

        absoluteDimensions.bottom = absoluteDimensions.top + height;
        absoluteDimensions.right = absoluteDimensions.left + width;

        for (uint32 row = absoluteDimensions.top; row <= absoluteDimensions.bottom; ++row) {
            for (uint32 col = absoluteDimensions.left; col <= absoluteDimensions.right; ++col) {
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
        auto& roomDimensions = rooms[roomIndex].dimensions;
        roomDimensions.top = min(row - 1, roomDimensions.top);
        roomDimensions.left = min(col - 1, roomDimensions.left);
        roomDimensions.bottom = max(row + 1, roomDimensions.bottom);
        roomDimensions.right = max(col + 1, roomDimensions.right);

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

    void ProceduralLevel::CreateConnector(ProceduralRoom& start, ProceduralRoom& end)
    {
        vec2 delta = (end.center - start.center);


    }

    void ProceduralLevel::CreateTile(uint32 row, 
        uint32 col,
        FileResource modelFile)
    {
        auto& entityManager = Core::GetEntityManager();
        EntityId entityId = entityManager.CreateEntity();
        entities.Push(entityId);
        vec3 tilePosition = vec3((float)col, -0.5f, (float)row);

        entityManager.AddComponent<TransformComponent>(entityId,
            vec3(),
            vec3(TransformComponent::LogicalPointToWorld(tilePosition)),
            vec3(),
            vec3(1.0f));

        entityManager.AddComponent<ModelComponent>(entityId, modelFile);
    }

    void ProceduralLevel::CreateTexturedTile(uint32 row,
        uint32 col,
        FileResource modelFile,
        FileResource textureFile)
    {
        auto& entityManager = Core::GetEntityManager();
        EntityId entityId = entityManager.CreateEntity();
        entities.Push(entityId);
        vec3 tilePosition = vec3((float)col, -0.5f, (float)row);

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
        auto& inputStream = archetype.GetInputStream();
        archetypeDocument.ParseStream(rapidjson::IStreamWrapper(inputStream));
        AddComponents(entityManager, entityId, archetypeDocument);

        Core::PostEvent(EntityCreatedEvent(entityId, logicalPosition));
    }
}