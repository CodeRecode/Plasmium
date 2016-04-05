#pragma once
#include "Types.h"
#define _X86_
#include <windef.h>

namespace Plasmium
{
    class Window
    {
    private:
        HWND handle;
        HGLRC OGLRenderContext;
        const static int32 WINDOW_WIDTH = 500;
        const static int32 WINDOW_HEIGHT = 400;
        bool shouldQuit;

    public:
        Window();
        ~Window();

        bool ShouldQuit() { return shouldQuit; }
        void PumpMessages();
        void SwapBuffers();

        static void CreateConsole();
    };
}