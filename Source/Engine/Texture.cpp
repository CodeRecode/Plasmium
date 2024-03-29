#include "Texture.h"
#include "Window.h"
#include <d3dx11tex.h>

namespace Plasmium
{
    Texture::Texture(FileResource& file) : file(file), texture(nullptr)
    {
    }

    bool Texture::Initialize(ID3D11Device* device)
    {
        HRESULT result;
        result = D3DX11CreateShaderResourceViewFromFile(device, 
            file.GetFileName(),
            nullptr,
            nullptr,
            &texture,
            nullptr);
        
        if (FAILED(result)) {
            Window::WriteError("Failed to load texture for: ", file.GetFileName());
            return false;
        }

        return true;
    }
    void Texture::Release()
    {
        if (texture != nullptr) {
            texture->Release();
            texture = nullptr;
        }
    }
}
