#pragma once
#include "mat4.h"

namespace Plasmium::ShaderInternal {
    struct MatrixInfoRef
    {
        const mat4& projection;
        const mat4& view;
        const mat4& world;
        MatrixInfoRef(const mat4& projection, const mat4& view, const mat4& world) :
            projection(projection),
            view(view),
            world(world)
        {}
    };
    struct MatrixInfo
    {
        mat4 projection;
        mat4 view;
        mat4 world;
        MatrixInfo(mat4 projection, mat4 view, mat4 world) :
            projection(projection),
            view(view),
            world(world)
        {}
    };
    struct LightInfo
    {
        vec4 ambientColor;
        vec4 diffuseColor;
        vec3 lightDirection;
        float specularPower;
        vec4 specularColor;
    };
    struct CameraInfo
    {
        vec3 cameraPosition;
        float pad;
    };
    struct MaterialInfo
    {
        vec3 diffuseColor;
        float shininess;
        int useTexture;
        vec3 pad;
    };
}