#include "MaterialShader.h"
#include "Window.h"

using namespace Plasmium::ShaderInternal;

namespace Plasmium {

    void MaterialShader::Initialize(ID3D11Device* device)
    {
        HRESULT result;
        const char* shaderName = "Material";
        result = Shader::Initialize(device, shaderName);
        if (FAILED(result)) {
            return;
        }

        D3D11_BUFFER_DESC materialBufferDesc;
        ZeroMemory(&materialBufferDesc, sizeof(materialBufferDesc));

        materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        materialBufferDesc.ByteWidth = sizeof(MaterialInfo);
        materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        materialBufferDesc.MiscFlags = 0;
        materialBufferDesc.StructureByteStride = 0;

        result = device->CreateBuffer(&materialBufferDesc, nullptr, &materialBuffer);
        if (FAILED(result)) {
            Window::WriteError("Could not create material buffer");
            Window::WriteError(shaderName);
            return;
        }
    }

    void MaterialShader::SetMaterial(ID3D11DeviceContext* deviceContext, const Material& material)
    {
        HRESULT result;
        D3D11_MAPPED_SUBRESOURCE mappedResource;

        result = deviceContext->Map(materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if (FAILED(result)) {
            Window::WriteError("Failed to map material buffer in shader");
            return;
        }

        MaterialInfo* materialData = (MaterialInfo*)mappedResource.pData;
        materialData->diffuseColor = material.diffuseColor;
        materialData->shininess = material.shininess;
        deviceContext->Unmap(materialBuffer, 0);

        deviceContext->PSSetConstantBuffers(psBufferIndex, 1, &materialBuffer);
    }

    void MaterialShader::Release()
    {
        if (materialBuffer != nullptr) {
            materialBuffer->Release();
            materialBuffer = nullptr;
        }
        Shader::Release();
    }
}