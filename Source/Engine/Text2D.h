#pragma once
#include "Types.h"
#include "vec4.h"
#include "rect.h"

#include <string>

namespace Plasmium {
    class Text2D {
    private:
        wchar_t wideString[256];
        uint32 length;

        rect drawArea;
        vec4 drawColor;

    public:
        Text2D(const char* string, rect drawArea, vec4 drawColor);

        void ChangeString(const char* string);

        const wchar_t* GetString() const { return &wideString[0]; }
        uint32 GetLength() const { return length; }
        const rect& GetDrawArea() const { return drawArea; }
        const vec4& GetDrawColor() const { return drawColor; }
    };
}