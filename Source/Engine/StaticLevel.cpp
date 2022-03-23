#include "StaticLevel.h"

#include "AllComponents.h"
#include "Core.h"
#include "EntityManager.h"
#include "Window.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

namespace Plasmium {

    void StaticLevel::Load()
    {
        Deserialize(file.GetInputStream());
    }

    void StaticLevel::SerializeJSON(std::ofstream& output)
    {
        assert(false);
    }

    void StaticLevel::DeserializeJSON(std::ifstream& input)
    {
        rapidjson::Document document;
        document.ParseStream(rapidjson::IStreamWrapper(input));

        auto& entityManager = Core::GetEntityManager();

        auto& levelData = document["level_data"];
        SetDimensions(levelData["height"].GetUint(), levelData["width"].GetUint());

        auto& tileDefs = document["tile_defs"];
        // assume it's 3x3 compatible
        const char* floor3ModelFile = tileDefs["floor3"].GetString();
        for (int32 row = 0; row < (int32)GetHeight(); row += 3) {
            for (int32 col = 0; col < (int32)GetWidth(); col += 3) {
                EntityId entityId = entityManager.CreateEntity();
                entities.Push(entityId);
                vec3 tilePosition = vec3(col + 1.0f, -0.5f, row + 1.0f);

                entityManager.AddComponent<TransformComponent>(entityId,
                    vec3(tilePosition),
                    vec3(TransformComponent::LogicalPointToWorld(tilePosition)),
                    vec3(),
                    vec3(1.0f));

                entityManager.AddComponent<ModelComponent>(entityId,
                    FileResource(floor3ModelFile));
            }
        }

        auto& gameObjects = document["game_objects"];
        for (auto& gameObject : gameObjects.GetArray()) {
            EntityId entityId = entityManager.CreateEntity();
            entities.Push(entityId);

            auto& transformData = gameObject["transform"];
            vec3 logicalPosition = GetVec3FromArray(transformData["logical_position"].GetArray());
            vec3 position = GetVec3FromArray(transformData["position"].GetArray());
            vec3 rotation = GetVec3FromArray(transformData["rotation"].GetArray());
            vec3 scale = GetVec3FromArray(transformData["scale"].GetArray());

            entityManager.AddComponent<TransformComponent>(entityId,
                logicalPosition,
                position,
                rotation,
                scale);

            AddComponents(entityManager, entityId, gameObject);
            if (gameObject.HasMember("archetype")) {
                FileResource archetypeFile = FileResource(gameObject["archetype"].GetString());
                rapidjson::Document archetypeDocument;
                archetypeDocument.ParseStream(rapidjson::IStreamWrapper(archetypeFile.GetInputStream()));
                AddComponents(entityManager, entityId, archetypeDocument);
            }

            Core::PostEvent(EntityCreatedEvent(entityId, logicalPosition));
        }
    }
}