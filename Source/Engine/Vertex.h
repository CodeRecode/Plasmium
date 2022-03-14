#pragma once
#include "vec2.h"
#include "vec3.h"

namespace Plasmium {
    struct Vertex
    {
        vec3 position;
        vec2 texture;
        vec3 normal;

        Vertex() {}
        Vertex(vec3 position, vec2 texture, vec3 normal) : position(position), 
            texture(texture), 
            normal(normal) 
        {}
    };
}