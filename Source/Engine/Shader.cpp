#include "Shader.h"
#include "Types.h"
#include "Window.h"
#include <d3dcompiler.h>
#include <string>
#include <stdlib.h>
#include "CameraManager.h"
#include "Core.h"

using namespace Plasmium::ShaderInternal;

namespace Plasmium
{
    HRESULT Shader::Initialize(ID3D11Device* device, const char* shaderName)
    {
        HRESULT result;
        ID3DBlob* errorMessage;

        std::string fileName(shaderName);
        std::string vertexShaderFile = "Source\\Engine\\Shaders\\" + fileName + "Vertex.hlsl";
        std::string pixelShaderFile = "Source\\Engine\\Shaders\\" + fileName + "Pixel.hlsl";

        size_t dummy;
        wchar_t vertexShaderFileWide[256];
        mbstowcs_s(&dummy, vertexShaderFileWide, vertexShaderFile.c_str(), 256);

        wchar_t pixelShaderFileWide[256];
        mbstowcs_s(&dummy, pixelShaderFileWide, pixelShaderFile.c_str(), 256);

        ID3DBlob* vertexShaderBuffer;
        result = D3DCompileFromFile(
            vertexShaderFileWide,
            nullptr,
            nullptr,
            "main",
            "vs_5_0",
            D3D10_SHADER_ENABLE_STRICTNESS,
            0,
            &vertexShaderBuffer,
            &errorMessage);

        if (FAILED(result)) {
            Window::WriteError("Could not load vertex shader");
            Window::WriteError(vertexShaderFile.c_str());

            if (errorMessage != nullptr) {
                Window::WriteError((char*)errorMessage->GetBufferPointer());
            }
            return result;
        }

        ID3DBlob* pixelShaderBuffer;
        result = D3DCompileFromFile(
            pixelShaderFileWide,
            nullptr,
            nullptr,
            "main",
            "ps_5_0",
            D3D10_SHADER_ENABLE_STRICTNESS,
            0,
            &pixelShaderBuffer,
            &errorMessage);

        if (FAILED(result)) {
            Window::WriteError("Could not load pixel shader");
            Window::WriteError(pixelShaderFile.c_str());

            if (errorMessage != nullptr) {
                Window::WriteError((char*)errorMessage->GetBufferPointer());
            }
            return result;
        }

        result = device->CreateVertexShader(
            vertexShaderBuffer->GetBufferPointer(),
            vertexShaderBuffer->GetBufferSize(),
            nullptr,
            &vertexShader);

        if (FAILED(result)) {
            Window::WriteError("Could not create vertex shader");
            Window::WriteError(shaderName);
            return result;
        }

        result = device->CreatePixelShader(
            pixelShaderBuffer->GetBufferPointer(),
            pixelShaderBuffer->GetBufferSize(),
            nullptr,
            &pixelShader);

        if (FAILED(result)) {
            Window::WriteError("Could not create pixel shader");
            Window::WriteError(shaderName);
            return result;
        }

        D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
        ZeroMemory(&polygonLayout, sizeof(polygonLayout));
        uint32 layoutIndex = 0;
        polygonLayout[layoutIndex].SemanticName = "POSITION";
        polygonLayout[layoutIndex].SemanticIndex = 0;
        polygonLayout[layoutIndex].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        polygonLayout[layoutIndex].InputSlot = 0;
        polygonLayout[layoutIndex].AlignedByteOffset = 0;
        polygonLayout[layoutIndex].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[layoutIndex].InstanceDataStepRate = 0;
        ++layoutIndex;

        polygonLayout[layoutIndex].SemanticName = "TEXCOORD";
        polygonLayout[layoutIndex].SemanticIndex = 0;
        polygonLayout[layoutIndex].Format = DXGI_FORMAT_R32G32_FLOAT;
        polygonLayout[layoutIndex].InputSlot = 0;
        polygonLayout[layoutIndex].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        polygonLayout[layoutIndex].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[layoutIndex].InstanceDataStepRate = 0;
        ++layoutIndex;

        polygonLayout[layoutIndex].SemanticName = "NORMAL";
        polygonLayout[layoutIndex].SemanticIndex = 0;
        polygonLayout[layoutIndex].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        polygonLayout[layoutIndex].InputSlot = 0;
        polygonLayout[layoutIndex].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        polygonLayout[layoutIndex].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[layoutIndex].InstanceDataStepRate = 0;
        ++layoutIndex;

        result = device->CreateInputLayout(
            polygonLayout,
            layoutIndex,
            vertexShaderBuffer->GetBufferPointer(),
            vertexShaderBuffer->GetBufferSize(),
            &layout);

        if (FAILED(result)) {
            Window::WriteError("Could not create input layout");
            Window::WriteError(shaderName);
            return result;
        }

        vertexShaderBuffer->Release();
        pixelShaderBuffer->Release();

        D3D11_BUFFER_DESC matrixBufferDesc;
        ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));

        matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        matrixBufferDesc.ByteWidth = sizeof(MatrixInfo);
        matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        matrixBufferDesc.MiscFlags = 0;
        matrixBufferDesc.StructureByteStride = 0;

        result = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);
        if (FAILED(result)) {
            Window::WriteError("Could not create matrix buffer");
            Window::WriteError(shaderName);
            return result;
        }

        D3D11_BUFFER_DESC cameraBufferDesc;
        ZeroMemory(&cameraBufferDesc, sizeof(cameraBufferDesc));

        cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        cameraBufferDesc.ByteWidth = sizeof(CameraInfo);
        cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cameraBufferDesc.MiscFlags = 0;
        cameraBufferDesc.StructureByteStride = 0;

        result = device->CreateBuffer(&cameraBufferDesc, nullptr, &cameraBuffer);
        if (FAILED(result)) {
            Window::WriteError("Could not create camera buffer");
            Window::WriteError(shaderName);
            return result;
        }

        D3D11_BUFFER_DESC lightBufferDesc;
        ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));

        lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        lightBufferDesc.ByteWidth = sizeof(LightInfo);
        lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        lightBufferDesc.MiscFlags = 0;
        lightBufferDesc.StructureByteStride = 0;

        result = device->CreateBuffer(&lightBufferDesc, nullptr, &lightBuffer);
        if (FAILED(result)) {
            Window::WriteError("Could not create light buffer");
            Window::WriteError(shaderName);
            return result;
        }

        return result;
    }

    void Shader::Bind(ID3D11DeviceContext* deviceContext, const MatrixInfoRef& matrices)
    {
        HRESULT result;
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        vsBufferIndex = 0, psBufferIndex = 0;

        if (matrixBuffer != nullptr) {
            result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            if (FAILED(result)) {
                Window::WriteError("Failed to map matrix buffer in shader");
                return;
            }

            MatrixInfo* matrixData = (MatrixInfo*)mappedResource.pData;
            matrixData->projection = matrices.projection;
            matrixData->view = matrices.view;
            matrixData->world = matrices.world;
            deviceContext->Unmap(matrixBuffer, 0);
            deviceContext->VSSetConstantBuffers(vsBufferIndex++, 1, &matrixBuffer);
        }

        if (cameraBuffer != nullptr) {
            result = deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            if (FAILED(result)) {
                Window::WriteError("Failed to map camera buffer in shader");
                return;
            }
            auto* camera = Core::GetCameraManager().GetCamera();
            CameraInfo* cameraData = (CameraInfo*)mappedResource.pData;
            cameraData->cameraPosition = camera->GetPosition();
            deviceContext->Unmap(cameraBuffer, 0);
            deviceContext->VSSetConstantBuffers(vsBufferIndex++, 1, &cameraBuffer);
        }

        // Light Buffer
        result = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if (FAILED(result)) {
            Window::WriteError("Failed to map light buffer in shader");
            return;
        }

        if (cameraBuffer != nullptr) {
            LightInfo* lightData = (LightInfo*)mappedResource.pData;
            lightData->ambientColor = vec4(0.15f, 0.15f, 0.15f, 1.0f);
            lightData->diffuseColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
            lightData->lightDirection = vec3(-0.5f, 1.0f, -1.0f);
            lightData->specularPower = 32.0f;
            lightData->specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
            deviceContext->Unmap(lightBuffer, 0);
            deviceContext->PSSetConstantBuffers(psBufferIndex++, 1, &lightBuffer);
        }

        deviceContext->IASetInputLayout(layout);
        deviceContext->VSSetShader(vertexShader, nullptr, 0);
        deviceContext->PSSetShader(pixelShader, nullptr, 0);
    }

    void Shader::Release()
    {
        if (lightBuffer != nullptr) {
            lightBuffer->Release();
            lightBuffer = nullptr;
        }
        if (cameraBuffer != nullptr) {
            cameraBuffer->Release();
            cameraBuffer = nullptr;
        }
        if (matrixBuffer != nullptr) {
            matrixBuffer->Release();
            matrixBuffer = nullptr;
        }
        if (layout != nullptr) {
            layout->Release();
            layout = nullptr;
        }
        if (pixelShader != nullptr) {
            pixelShader->Release();
            pixelShader = nullptr;
        }
        if (vertexShader != nullptr) {
            vertexShader->Release();
            vertexShader = nullptr;
        }
    }
}