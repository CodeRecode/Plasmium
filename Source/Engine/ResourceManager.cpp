#include "ResourceManager.h"

#include "Core.h"
#include "Event.h"

namespace Plasmium {
    StaticLevel& ResourceManager::GetStaticLevelResource(FileResource levelFile)
    {
        auto id = levelFile.GetId();
        if (levels.Contains(id)) {
            return *levels.GetObjectPtr(id);
        }

        auto * newLevel = levels.EmplaceObject(id, StaticLevel(levelFile));
        return *newLevel;
    }


    Model& ResourceManager::GetModelResource(FileResource modelFile)
    {
        auto modelId = modelFile.GetId();
        if (!models.Contains(modelId)) {
            Model* model = models.EmplaceObject(modelId, Model(modelFile));
            Core::GetInstance().PostEvent(ModelLoadedEvent(modelFile));
        }

        return *models.GetObjectPtr(modelId);
    }

    Texture& ResourceManager::GetTextureResource(FileResource textureFile)
    {
        auto textureId = textureFile.GetId();
        if (!textures.Contains(textureId)) {
            Texture* texture = textures.EmplaceObject(textureId, Texture(textureFile));
            Core::GetInstance().PostEvent(TextureLoadedEvent(textureFile));
        }

        return *textures.GetObjectPtr(textureId);
    }

    const PlasString& ResourceManager::GetString(StringId stringId)
    {
        return *strings.GetObjectPtr(stringId);
    }

    StringId ResourceManager::CreateString(const char* string)
    {
        StringId id = HashFunc<const char*>(string);
        if (strings.Contains(id)) {
            return id;
        }
        
        strings.EmplaceObject(id, PlasString(string));
        return id;
    }
}