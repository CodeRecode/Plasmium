#include "Model.h"
#include "Shader.h"
#include "Window.h"
#include <sstream>
#include <ofbx.h>

namespace Plasmium
{
    Model::Model(FileResource file) : file(file),
        vertexBuffer(nullptr)
    { }

    bool Model::Initialize(ID3D11Device* device)
    {
        HRESULT result;

        auto extensionType = file.GetFileExtensionType();
        ModelLoadResult loadResult;
        if (extensionType == FileExentionType::OBJ) {
            loadResult = GetOBJ();
        }
        else if (extensionType == FileExentionType::FBX) {
            loadResult = GetFBX();
        }
        else {
            std::string error = "Unrecognized model extension: ";
            Window::WriteError(error + std::to_string((int32)extensionType));
            return false;
        }

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(Vertex) * loadResult.vertices.Size();
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA vertexData;
        ZeroMemory(&vertexData, sizeof(vertexData));
        vertexData.pSysMem = loadResult.vertices.begin();
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
        if (FAILED(result)) {
            std::string error = "Failed to create vertex buffer for: ";
            Window::WriteError(error + GetFileName());
            return false;
        }

        uint32 indexBufferCount = loadResult.materials.Size();
        for (uint32 materialIndex = 0; materialIndex < indexBufferCount; ++materialIndex) {
            Array<uint32> indicesToUse;
            for (uint32 i = 0; i < loadResult.materialIndices.Size(); ++i) {
                if (loadResult.materialIndices[i] == materialIndex) {
                    uint32 triangleIndex = i * 3;
                    indicesToUse.Push(triangleIndex);
                    indicesToUse.Push(triangleIndex + 1);
                    indicesToUse.Push(triangleIndex + 2);
                }
            }
            if (indicesToUse.Size() == 0) {
                continue;
            }

            MaterialIndexBuffer mib;
            mib.indexCount = indicesToUse.Size();
            mib.material = loadResult.materials[materialIndex];

            D3D11_BUFFER_DESC indexBufferDesc;
            ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufferDesc.ByteWidth = sizeof(uint32) * indicesToUse.Size();
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;
            indexBufferDesc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA indexData;
            ZeroMemory(&indexData, sizeof(indexData));
            indexData.pSysMem = indicesToUse.begin();
            indexData.SysMemPitch = 0;
            indexData.SysMemSlicePitch = 0;

            result = device->CreateBuffer(&indexBufferDesc, &indexData, &mib.indexBuffer);
            if (FAILED(result)) {
                std::string error = "Failed to create index buffer for: ";
                Window::WriteError(error + GetFileName());
                return false;
            }

            matIndexBuffers.Push(mib);
        }

        return true;
    }

    void Model::Draw(ID3D11DeviceContext* deviceContext, Shader* shader) const
    {
        uint32 stride = sizeof(Vertex);
        uint32 offset = 0;

        deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

        for (uint32 index = 0; index < matIndexBuffers.Size(); ++index) {
            auto& matIndexBuffer = matIndexBuffers[index];
            shader->SetMaterial(deviceContext, matIndexBuffer.material);
            deviceContext->IASetIndexBuffer(matIndexBuffer.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
            deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            deviceContext->DrawIndexed(matIndexBuffer.indexCount, 0, 0);
        }
    }

    void Model::Release()
    {
        for (auto& matIndexBuffer : matIndexBuffers) {
            matIndexBuffer.indexBuffer->Release();
        }
        matIndexBuffers.Clear();

        if (vertexBuffer != nullptr) {
            vertexBuffer->Release();
            vertexBuffer = 0;
        }
    }

    Model::ModelLoadResult Model::GetOBJ() {
        auto fileStream = file.GetInputStream();
        ModelLoadResult result;

        Array<vec3> vertices;
        Array<vec2> texCoords;
        Array<vec3> normals;

        std::string line;
        while (std::getline(fileStream, line))
        {
            if (line.empty())
                continue;
            if (line[0] == 'v') {
                std::stringstream sstream(line);
                std::string x, y, z;
                sstream >> x; // Discard v*
                if (line[1] == ' ') {
                    sstream >> x >> y >> z;
                    vertices.Push(vec3(std::stof(x), std::stof(y), std::stof(z)));
                }
                else if (line[1] == 't') {
                    sstream >> x >> y;
                    texCoords.Push(vec2(std::stof(x), std::stof(y)));
                }
                else if (line[1] == 'n') {
                    sstream >> x >> y >> z;
                    normals.Push(vec3(std::stof(x), std::stof(y), std::stof(z)));
                }
            }
            else if (line[0] == 'f') {
                std::string face;
                for (uint32 i = 0; i < line.size(); ++i) {
                    if (line[i] == '/') {
                        line[i] = ' ';
                    }
                }
                std::stringstream sstream(line);
                sstream >> face; // Discard f
                int vertexIndex, texCoordIndex, normalIndex;

                for (int i = 0; i < 3; ++i) {
                    sstream >> vertexIndex >> texCoordIndex >> normalIndex;
                    result.vertices.Push(Vertex(vertices[vertexIndex - 1],
                        texCoords[texCoordIndex - 1],
                        normals[normalIndex - 1]));
                }
            }
        }

        // Push dummy material
        result.materials.Push(Material(vec3(1.0f, 1.0f, 0.0f), 16.0f));
        for (uint32 i = 0; i < result.vertices.Size() / 3; ++i) {
            result.materialIndices.Push(0);
        }

        return result;
    }

    vec2 to_vec2(const ofbx::Vec2& input) {
        return vec2((float)input.x, (float)input.y);
    }

    vec3 to_vec3(const ofbx::Vec3& input) {
        return vec3((float)input.x, (float)input.y, (float)input.z);
    }

    vec3 to_vec3(const ofbx::Color& input) {
        return vec3((float)input.r, (float)input.g, (float)input.b);
    }

    Model::ModelLoadResult Model::GetFBX()
    {
        ModelLoadResult result;
        FILE* input;
        fopen_s(&input, file.GetFileName(), "rb");
        if (input == nullptr) {
            std::string error = "Failed to open file for: ";
            Window::WriteError(error + GetFileName());
            return result;
        }

        fseek(input, 0, SEEK_END);
        uint32 fileSize = (uint32)ftell(input);
        fseek(input, 0, SEEK_SET);

        char* fileContents = new char[fileSize];
        fread(fileContents, 1, fileSize, input);

        auto* scene = ofbx::load((ofbx::u8*)fileContents, 
            fileSize, 
            (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
        
        delete[] fileContents;
        fclose(input);

        if (scene == nullptr) {
            std::string error = "Failed to read fbx file for: ";
            Window::WriteError(error + GetFileName());
            return result;
        }

        uint32 meshCount = scene->getMeshCount();
        auto objects = scene->getAllObjects();
        for (uint32 meshIndex = 0; meshIndex < meshCount; ++meshIndex) {
            auto& mesh = *scene->getMesh(meshIndex);
            auto& geometry = *mesh.getGeometry();
            vec3 translation = to_vec3(mesh.getLocalTranslation());
            auto* parent = mesh.getParent();
            while (parent != nullptr) {
                translation += to_vec3(parent->getLocalTranslation());
                parent = parent->getParent();
            }            

            auto* positions = geometry.getVertices();
            auto* normals = geometry.getNormals();
            auto* texCoords = geometry.getUVs();
            auto* faceIndices = geometry.getFaceIndices();

            for (int32 vertexIndex = 0; vertexIndex < geometry.getIndexCount(); ++vertexIndex) {
                int32 faceIndex = faceIndices[vertexIndex];
                faceIndex = faceIndex < 0 ? -(faceIndex + 1) : faceIndex;
                auto& position = positions[faceIndex];
                result.vertices.Push(Vertex(to_vec3(position) + translation,
                    to_vec2(texCoords[vertexIndex]), 
                    to_vec3(normals[vertexIndex])));
            }

            auto* materialIndices = geometry.getMaterials();
            int32 materialIndexCount = geometry.getIndexCount() / 3; // Material index is per triangle
            uint32 materialOffset = result.materials.Size();
            for (int32 materialIIndex = 0; materialIIndex < materialIndexCount; ++materialIIndex) {
                if (materialIndices == nullptr) {
                    result.materialIndices.Push(materialOffset);
                }
                else {
                    result.materialIndices.Push(materialIndices[materialIIndex] + materialOffset);
                }
            }

            for (int32 materialIndex = 0; materialIndex < mesh.getMaterialCount(); ++materialIndex) {
                auto& material = *mesh.getMaterial(materialIndex);
                result.materials.Push(Material(to_vec3(material.getDiffuseColor()), 
                    (float)material.getShininess()));
            }
        }


        scene->destroy();
        return result;
    }
}