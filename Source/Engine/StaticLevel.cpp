#include "StaticLevel.h"
#include "Window.h"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "Core.h"

namespace Plasmium {

    void StaticLevel::Load()
    {
        Deserialize(file.GetInputStream());
    }

    void StaticLevel::Unload()
    {
        auto& entityManager = Core::GetInstance().GetEntityManager();
        for (EntityId entity : entities) {
            entityManager.DeleteEntity(entity);
        }
        entities.Clear();
    }

    void StaticLevel::SerializeJSON(std::ofstream& output)
    {
        assert(false);
    }

    vec3 GetVecFromArray(const rapidjson::GenericArray<false, rapidjson::Value>& arr)
    {
        return vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
    }

    void StaticLevel::DeserializeJSON(std::ifstream& input)
    {
        rapidjson::Document document;
        document.ParseStream(rapidjson::IStreamWrapper(input));

        auto& entityManager = Core::GetInstance().GetEntityManager();

        auto& levelData = document["level_data"];
        vec3 cameraPos = GetVecFromArray(levelData["camera_position"].GetArray());
        vec3 cameraRot = GetVecFromArray(levelData["camera_rotation"].GetArray());
        Core::GetInstance().PostEvent(MoveCameraEvent(
            cameraPos, 
            cameraRot));

        height = levelData["height"].GetUint();
        width = levelData["width"].GetUint();
        for (uint32 row = 0; row < height; ++row) {
            Array<Tile> tiles;
            for (uint32 col = 0; col < width; ++col) {
                tiles.Push(Tile());
            }
            map.Push(tiles);
        }

        auto& tileDefs = document["tile_defs"];
        // assume it's 3x3 compatible
        const char* floor3ModelFile = tileDefs["floor3"].GetString();
        for (int32 row = 0; row < (int32)height; row += 3) {
            for (int32 col = 0; col < (int32)width; col += 3) {
                auto* entity = entityManager.CreateEntity();
                entities.Push(entity->GetId());
                vec3 tilePosition = vec3(col + 1.0f, -0.5f, row + 1.0f);

                entityManager.AddComponent(entity,
                    ComponentType::Transform,
                    vec3(tilePosition),
                    vec3(TransformComponent::LogicalPointToWorld(tilePosition)),
                    vec3(),
                    vec3(1.0f));

                entityManager.AddComponent(entity,
                    ComponentType::Model,
                    FileResource(floor3ModelFile));
            }
        }

        auto& gameObjects = document["game_objects"];
        for (auto& gameObject : gameObjects.GetArray()) {
            auto* entity = entityManager.CreateEntity();
            entities.Push(entity->GetId());

            const char* modelFile = gameObject["model"].GetString();
            if (gameObject.HasMember("texture")) {
                const char* texture = gameObject["texture"].GetString();
                entityManager.AddComponent(entity,
                    ComponentType::Model,
                    FileResource(modelFile),
                    FileResource(texture));
            }
            else {
                entityManager.AddComponent(entity,
                    ComponentType::Model,
                    FileResource(modelFile));
            }

            if (gameObject.HasMember("player_controller")) {
                entityManager.AddComponent(entity,
                    ComponentType::PlayerController);
            }

            vec3 logicalPosition = GetVecFromArray(gameObject["logical_position"].GetArray());
            vec3 position = GetVecFromArray(gameObject["position"].GetArray());
            vec3 rotation = GetVecFromArray(gameObject["rotation"].GetArray());
            vec3 scale = GetVecFromArray(gameObject["scale"].GetArray());

            entityManager.AddComponent(entity,
                ComponentType::Transform,
                logicalPosition,
                position,
                rotation,
                scale);

            // Only supports 90 degree rotations
            if (gameObject.HasMember("collider")) {
                auto& colliderData = gameObject["collider"];
                float rotationAngle = rotation.y;
                uint32 colliderHeight = colliderData["height"].GetUint();
                uint32 colliderWidth = colliderData["width"].GetUint();
                vec3 start = GetVecFromArray(colliderData["start"].GetArray());

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
                        square += logicalPosition + start;
                        map[(uint32)square.z][(uint32)square.x].SetCollision();
                    }
                }
            }
        }
    }
}