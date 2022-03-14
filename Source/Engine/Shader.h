#pragma once
#include <d3d11.h>
#include "mat4.h"
#include "Material.h"
#include "Camera.h"
#include "ShaderTypes.h"

namespace Plasmium
{
    class Shader
    {
    protected:
        ID3D11VertexShader* vertexShader = nullptr;
        ID3D11PixelShader* pixelShader = nullptr;
        ID3D11InputLayout* layout = nullptr;
        ID3D11Buffer* matrixBuffer = nullptr;
        ID3D11Buffer* cameraBuffer = nullptr;
        ID3D11Buffer* lightBuffer = nullptr;

        uint32 vsBufferIndex = 0;
        uint32 psBufferIndex = 0;
        HRESULT Initialize(ID3D11Device* device, const char* shaderName);

    public:
        virtual void Bind(ID3D11DeviceContext* deviceContext, const ShaderInternal::MatrixInfo& matrices);
        virtual void SetMaterial(ID3D11DeviceContext* deviceContext, const Material& material) {}
        virtual void Release();
    };
}