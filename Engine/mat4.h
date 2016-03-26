#pragma once
#include "Types.h"
#include "vec4.h"

namespace Plasmium
{
    class mat4
    {
    private:
        vec4 data[4];

    public:
        mat4() { };
        mat4(float val);

        vec4& operator[](uint32 index);
    };
}