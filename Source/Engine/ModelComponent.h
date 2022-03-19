#pragma once

#include "Component.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"

namespace Plasmium {
    class ModelComponent : public Component<ModelComponent>
    {
    private:
        FileResource modelFile;
        FileResource textureFile;

    public:
        ModelComponent(EntityId entityId, FileResource modelFile);
        ModelComponent(EntityId entityId, FileResource modelFile, FileResource textureFile);
        void Draw(ID3D11DeviceContext* deviceContext, Shader* shader) const;

        bool HasTexture() const { return !textureFile.IsNone(); }
    };
    ComponentType ModelComponent::type = ComponentType::Model;
}