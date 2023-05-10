#include "Level.h"

#include "AllComponents.h"
#include "Core.h"
#include "EntityManager.h"
#include "Window.h"


namespace Plasmium {

    void Level::Unload()
    {
        entities.Clear();
        map.Clear();
    }

    void Level::SetDimensions(uint32 height, uint32 width)
    {
        map.Clear();
        this->height = height;
        this->width = width;

        for (uint32 row = 0; row < height; ++row) {
            Array<Tile> tiles;
            for (uint32 col = 0; col < width; ++col) {
                tiles.Push(Tile());
            }
            map.Push(std::move(tiles));
        }

    }

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

    vec3 Level::GetVec3FromArray(const rapidjson::GenericArray<false, rapidjson::Value>& arr)
    {
        return vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
    }

    void Level::AddComponents(EntityManager& entityManager, 
        EntityId entityId,
        rapidjson::Value& jsonValue)
    {
        auto& transform = *entityManager.GetComponent<TransformComponent>(entityId);

        if (jsonValue.HasMember("model")) {
            const char* modelFile = jsonValue["model"].GetString();
            if (jsonValue.HasMember("texture")) {
                const char* texture = jsonValue["texture"].GetString();
                entityManager.AddComponent<ModelComponent>(entityId,
                    FileResource(modelFile),
                    FileResource(texture));
            }
            else {
                entityManager.AddComponent<ModelComponent>(entityId,
                    FileResource(modelFile));
            }
        }

        if (jsonValue.HasMember("camera")) {
            auto& cameraData = jsonValue["camera"];
            entityManager.AddComponent<CameraComponent>(entityId,
                transform.GetPosition(),
                GetVec3FromArray(cameraData["position"].GetArray()),
                GetVec3FromArray(cameraData["rotation"].GetArray()));
        }

        if (jsonValue.HasMember("name")) {
            entityManager.AddComponent<NameComponent>(entityId,
                jsonValue["name"].GetString());
        }

        if (jsonValue.HasMember("player_controller")) {
            entityManager.AddComponent<PlayerControllerComponent>(entityId);
        }

        if (jsonValue.HasMember("monster_controller")) {
            entityManager.AddComponent<MonsterControllerComponent>(entityId);
        }

        if (jsonValue.HasMember("combat")) {
            auto& combatData = jsonValue["combat"];
            float health = combatData["health"].GetFloat();
            float damage = combatData["damage"].GetFloat();
            entityManager.AddComponent<CombatComponent>(entityId, health, damage);
        }

        // Only supports 90 degree rotations
        if (jsonValue.HasMember("collider")) {
            auto& colliderData = jsonValue["collider"];
            float rotationAngle = transform.GetRotation().y;
            uint32 colliderHeight = colliderData["height"].GetUint();
            uint32 colliderWidth = colliderData["width"].GetUint();
            vec3 start = GetVec3FromArray(colliderData["start"].GetArray());

            if (rotationAngle == 90.0f || rotationAngle == -90.0f) {
                uint32 temp = colliderHeight;
                colliderHeight = colliderWidth;
                colliderWidth = temp;
                float temp2 = start.x;
                start.x = start.z;
                start.z = temp2;
            }
            if (rotationAngle == 180.0f) {
                start = start * -1.0f;
            }
            for (uint32 row = 0; row < colliderHeight; ++row) {
                for (uint32 col = 0; col < colliderWidth; ++col) {
                    vec3 square((float)col, 0, (float)row);
                    square += transform.GetLogicalPosition() + start;
                    map[(uint32)square.z][(uint32)square.x].SetGeometry(TileGeometry::Wall);
                }
            }
        }
    }


    bool Level::IsWalkable(vec3 logicalPostion)
    {
       return IsPositionInBounds(logicalPostion) && GetTile(logicalPostion).IsWalkable();
    }

    bool Level::HasWall(vec3 logicalPostion)
    {
        return IsPositionInBounds(logicalPostion) && GetTile(logicalPostion).GetGeometry() == TileGeometry::Wall;
    }

    bool Level::HasCreature(vec3 logicalPostion)
    {
        return IsPositionInBounds(logicalPostion) && GetTile(logicalPostion).HasCreature();
    }

    EntityId Level::GetCreature(vec3 logicalPostion)
    {
        auto& tile = GetTile(logicalPostion);
        assert(tile.HasCreature());
        return tile.GetCreature();
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