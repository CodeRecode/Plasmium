#pragma once
#include <Windows.h>
#include "GL/glew.h"
#include <GL/GL.h>
#include "Shader.h"


namespace Plasmium
{
    class Renderer
    {
    private:
        HWND hWnd;
        HGLRC OGLRenderContext;
        Shader shader; 
        GLuint vao;

    public:
        const static int WINDOW_WIDTH = 500;
        const static int WINDOW_HEIGHT = 400;

        void Initialize(HWND hWnd);
        void RenderFrame();
        void Cleanup();
    };
}