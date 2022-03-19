#pragma once
#include "CoreSystem.h"
#include "FileResource.h"
#include "Handler.h"
#include "Model.h"
#include "PlasString.h"
#include "StaticLevel.h"
#include "Types.h"

namespace Plasmium {
    class ResourceManager : public CoreSystem {
    private:
        Handler<FileId, StaticLevel> levels;
        Handler<FileId, Model> models;
        Handler<FileId, Texture> textures;
        Handler<StringId, PlasString> strings;

    public:
        const Array<Model>& GetAllModels() { return models.GetObjectsReference(); }
        const Array<Texture>& GetAllTextures() { return textures.GetObjectsReference(); }

        StaticLevel& GetStaticLevelResource(FileResource levelFile);
        Model& GetModelResource(FileResource modelFile);
        Texture& GetTextureResource(FileResource textureFile);
        const PlasString& GetString(StringId stringId);

        StringId CreateString(const char* string);
    };
}

