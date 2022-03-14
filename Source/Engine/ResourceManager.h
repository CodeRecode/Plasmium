#pragma once
#include <string>

#include "FileResource.h"
#include "Handler.h"
#include "StaticLevel.h"
#include "Model.h"
#include "Types.h"
#include "CoreSystem.h"

namespace Plasmium {
    class ResourceManager : public CoreSystem {
    private:
        Handler<FileId, StaticLevel> levels;
        Handler<FileId, Model> models;
        Handler<FileId, Texture> textures;
    public:
        const Array<Model>& GetAllModels() { return models.GetObjectsReference(); }
        const Array<Texture>& GetAllTextures() { return textures.GetObjectsReference(); }

        StaticLevel& GetStaticLevelResource(FileResource levelFile);
        Model& GetModelResource(FileResource modelFile);
        Texture& GetTextureResource(FileResource textureFile);
    };
}

