#include "mat4.h"

namespace Plasmium
{
    mat4::mat4(float val)
    {
        data[0][0] = data[1][1] = data[2][2] = data[3][3] = val;
    }

    vec4& mat4::operator[](uint32 index)
    {
        return data[index];
    }
}