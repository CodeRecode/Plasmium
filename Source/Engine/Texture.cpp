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
            file.GetFileName().c_str(),
            nullptr,
            nullptr,
            &texture,
            nullptr);
        
        if (FAILED(result)) {
            std::string error = "Failed to load texture for: ";
            Window::WriteError(error + file.GetFileName());
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
