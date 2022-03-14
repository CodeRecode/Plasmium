#pragma once
#include <d3d11.h>
#include "Material.h"
#include "Shader.h"

namespace Plasmium
{
    class MaterialShader : public Shader
    {
    private:
        ID3D11Buffer* materialBuffer = nullptr;
    public:
        void Initialize(ID3D11Device* device);
        void SetMaterial(ID3D11DeviceContext* deviceContext, const Material& material) override;
        void Release() override;
    };
}