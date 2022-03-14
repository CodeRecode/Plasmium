#pragma once
#include "vec2.h"
#include "vec3.h"

namespace Plasmium {
    struct Material
    {
        vec3 diffuseColor;
        float shininess;


        Material() {}
        Material(vec3 diffuseColor, float shininess) : diffuseColor(diffuseColor),
            shininess(shininess)
        {}
    };
}
