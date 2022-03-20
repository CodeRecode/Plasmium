#pragma once
#include "FileResource.h"
#include "Handler.h"
#include "Vertex.h"
#include "Material.h"
#include "Shader.h"
#include <d3d11.h>

#include "vec2.h"
#include "vec3.h"

namespace Plasmium
{
    struct MaterialIndexBuffer {
        Material material;
        ID3D11Buffer* indexBuffer;
        uint32 indexCount;
    };

    class Model {
    private:
        struct ModelLoadResult {
            Array<Vertex> vertices;
            Array<Material> materials;
            Array<uint32> materialIndices;
        };

        FileResource file;
        ID3D11Buffer* vertexBuffer;
        Array<MaterialIndexBuffer> matIndexBuffers;

        ModelLoadResult GetOBJ();
        ModelLoadResult GetFBX();

    public:
        Model(FileResource file);
        const char* GetFileName() { return file.GetFileName(); }
        FileId GetId() const { return file.GetId(); }

        bool Initialize(ID3D11Device* device);
        void Draw(ID3D11DeviceContext* deviceContext, Shader* shader) const;
        void Release();

    };
}