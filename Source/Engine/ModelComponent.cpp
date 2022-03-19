#include "ModelComponent.h"

#include "Core.h"
#include "ResourceManager.h"

namespace Plasmium
{
    ModelComponent::ModelComponent(EntityId entityId, FileResource modelFile) :
        Component(entityId),
        modelFile(modelFile),
        textureFile(FileResource::None())
    {
    }

    ModelComponent::ModelComponent(EntityId entityId, FileResource modelFile, FileResource textureFile) :
        Component(entityId),
        modelFile(modelFile),
        textureFile(textureFile)
    {
    }


    void ModelComponent::Draw(ID3D11DeviceContext* deviceContext, Shader* shader) const
    {
        auto& resourceManager = Core::GetInstance().GetResourceManager();
        auto& model = resourceManager.GetModelResource(modelFile);

        if (HasTexture()) {
            auto& texture = resourceManager.GetTextureResource(textureFile);
            auto* textureValue = texture.GetTexture();
            deviceContext->PSSetShaderResources(0, 1, &textureValue);
        }

        model.Draw(deviceContext, shader);
    }
}