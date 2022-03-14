#pragma once
#include <d3d11.h>
#include "Shader.h"

namespace Plasmium
{
    class TextureShader : public Shader
    {
    private:
        ID3D11SamplerState* samplerState = nullptr;
    public:
        void Initialize(ID3D11Device* device);
        void Bind(ID3D11DeviceContext* deviceContext, const ShaderInternal::MatrixInfo& matrices) override;
        void Release() override;
    };
}