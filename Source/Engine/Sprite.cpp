#include "Sprite.h"

#include "Core.h"
#include "ResourceManager.h"
#include "Window.h"

namespace Plasmium {


    void Sprite::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
    {
        HRESULT result;
        SpriteVertex* vertices = new SpriteVertex[VertexCount];

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        vertexBufferDesc.ByteWidth = sizeof(SpriteVertex) * VertexCount;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA vertexData;
        ZeroMemory(&vertexData, sizeof(vertexData));
        vertexData.pSysMem = vertices;
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
        delete[] vertices;
        if (FAILED(result)) {
            Window::WriteError("Could not create vertex buffer for sprite shader");
            return;
        }

        uint32* indices = new uint32[VertexCount];
        for (uint32 i = 0; i < VertexCount; ++i) {
            indices[i] = i;
        }

        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(uint32) * VertexCount;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA indexData;
        ZeroMemory(&indexData, sizeof(indexData));
        indexData.pSysMem = indices;
        indexData.SysMemPitch = 0;
        indexData.SysMemSlicePitch = 0;

        result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
        delete[] indices;
        if (FAILED(result)) {
            Window::WriteError("Could not create index buffer for sprite shader");
            return;
        }
        AdjustArea(deviceContext);
    }

    void Sprite::AdjustArea(ID3D11DeviceContext* deviceContext)
    {
        const auto& window = Core::GetInstance().GetWindow();
        // Map from (0,0) to (-w/2, h/2)
        rect screenSpace;
        screenSpace.x = ((float)window.GetWidth() / -2.0f) + area.x;
        screenSpace.width = screenSpace.x + area.width;
        screenSpace.y = ((float)window.GetHeight() / 2.0f) - area.y;
        screenSpace.height = screenSpace.y - area.height;

        vec3 topLeft = vec3(screenSpace.x, screenSpace.y, 0.0f);
        vec3 topRight = vec3(screenSpace.width, screenSpace.y, 0.0f);
        vec3 bottomLeft = vec3(screenSpace.x, screenSpace.height, 0.0f);
        vec3 bottomRight = vec3(screenSpace.width, screenSpace.height, 0.0f);

        HRESULT result;
        Array<SpriteVertex> vertices;
        vertices.Push(SpriteVertex(topLeft, vec2(0.0f, 0.0f)));
        vertices.Push(SpriteVertex(bottomRight, vec2(1.0f, 1.0f)));
        vertices.Push(SpriteVertex(bottomLeft, vec2(0.0f, 1.0f)));

        vertices.Push(SpriteVertex(topLeft, vec2(0.0f, 0.0f)));
        vertices.Push(SpriteVertex(topRight, vec2(1.0f, 0.0f)));
        vertices.Push(SpriteVertex(bottomRight, vec2(1.0f, 1.0f)));

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if (FAILED(result)) {
            Window::WriteError("Could not map vertex buffer for sprite shader");
            return;
        }
        SpriteVertex* data = (SpriteVertex*)mappedResource.pData;
        memcpy(data, vertices.begin(), sizeof(SpriteVertex) * VertexCount);
        deviceContext->Unmap(vertexBuffer, 0);
    }

    void Sprite::Draw(ID3D11DeviceContext* deviceContext) const
    {
        auto& resourceManager = Core::GetInstance().GetResourceManager();
        auto& texture = resourceManager.GetTextureResource(textureFile);
        auto* textureValue = texture.GetTexture();
        deviceContext->PSSetShaderResources(0, 1, &textureValue);

        uint32 stride = sizeof(SpriteVertex);
        uint32 offset = 0;

        deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->DrawIndexed(VertexCount, 0, 0);
    }

    void Sprite::Release()
    {
        if (indexBuffer != nullptr) {
            indexBuffer->Release();
            indexBuffer = nullptr;
        }
        if (vertexBuffer != nullptr) {
            vertexBuffer->Release();
            vertexBuffer = nullptr;
        }
    }
}