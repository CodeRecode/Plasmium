#pragma once
#include <d3d11.h>
#include "rect.h"
#include "FileResource.h"

namespace Plasmium
{
    class Sprite {
    private:
        struct SpriteVertex {
            vec3 postion;
            vec2 texCoord;
            SpriteVertex() {}
            SpriteVertex(vec3 postion, vec2 texCoord) :
                postion(postion),
                texCoord(texCoord)
            {}
        };

        ID3D11Buffer* vertexBuffer = nullptr;
        ID3D11Buffer* indexBuffer = nullptr;

        const uint32 VertexCount = 6;
        FileResource textureFile;
        rect<> area;

        void AdjustArea(ID3D11DeviceContext* deviceContext);

    public:
        Sprite(FileResource textureFile, rect<> area) :
            textureFile(textureFile),
            area(area)
        {}
        const FileResource& GetTextureFile() const { return textureFile; }
        const rect<>& GetRect() const { return area; }

        void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
        void Draw(ID3D11DeviceContext* deviceContext) const;
        void Release();
    };
}