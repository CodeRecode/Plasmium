#include "TextureShader.h"
#include "Window.h"

using namespace Plasmium::ShaderInternal;

namespace Plasmium {

    void TextureShader::Initialize(ID3D11Device* device)
    {
        HRESULT result;
        const char* shaderName = "Texture";
        result = Shader::Initialize(device, shaderName);
        if (FAILED(result)) {
            return;
        }

        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        // samplerDesc.BorderColor;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        result = device->CreateSamplerState(&samplerDesc, &samplerState);
        if (FAILED(result)) {
            Window::WriteError("Could not create sampler state");
            Window::WriteError(shaderName);
            return;
        }
    }

    void TextureShader::Bind(ID3D11DeviceContext* deviceContext, const MatrixInfo& matrices)
    {
        Shader::Bind(deviceContext, matrices);
        deviceContext->PSSetSamplers(0, 1, &samplerState);
    }

    void TextureShader::Release()
    {
        if (samplerState != nullptr) {
            samplerState->Release();
            samplerState = nullptr;
        }
        Shader::Release();
    }
}