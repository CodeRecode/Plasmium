#include "Text2D.h"

#include <cstring>

namespace Plasmium {
    Text2D::Text2D(const char* string, rect drawArea, vec4 drawColor) :
        drawArea(drawArea),
        drawColor(drawColor)
    {
        ChangeString(string);
    }

    void Text2D::ChangeString(const char* string)
    {
        mbstowcs_s(&length, wideString, string, 256);
    }
}