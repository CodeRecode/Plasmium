#pragma once
#include <d3d11.h>
#include "Shader.h"

#include "rect.h"
#include "vec2.h"
#include "vec3.h"

namespace Plasmium
{
    class SpriteShader : public Shader {
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
        ID3D11SamplerState* samplerState = nullptr;

        rect<> lastArea;

    public:
        void Initialize(ID3D11Device* device);
        void Bind(ID3D11DeviceContext* deviceContext, const ShaderInternal::MatrixInfoRef& matrices);
        void Release() override;
    };
}