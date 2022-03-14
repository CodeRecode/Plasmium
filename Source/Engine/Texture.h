#pragma once
#include "FileResource.h"
#include <d3d11.h>

namespace Plasmium
{
    class Texture {
    private:
        FileResource file;
        ID3D11ShaderResourceView* texture;

    public:
        Texture(FileResource& file);

        bool Initialize(ID3D11Device* device);
        void Release();

        ID3D11ShaderResourceView* GetTexture() { return texture; }
    };
}