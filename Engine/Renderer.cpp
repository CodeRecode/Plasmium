#include "Types.h"
#include "Renderer.h"

namespace Plasmium
{
    void Renderer::Initialize(HWND hWnd)
    {
        this->hWnd = hWnd;

        // Initialize D3D
        PIXELFORMATDESCRIPTOR pixelFormatDesc = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,            // Type of buffer
            32,                       // Color 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            24,                       // Depth buffer bits
            8,                        // Stencil buffer bits
            0,                        // Aux buffers
            PFD_MAIN_PLANE,
            0, 0, 0, 0
        };

        HDC deviceContextHandle = GetDC(hWnd);

        int32 pixelFormat = ChoosePixelFormat(deviceContextHandle, &pixelFormatDesc);
        SetPixelFormat(deviceContextHandle, pixelFormat, &pixelFormatDesc);

        OGLRenderContext = wglCreateContext(deviceContextHandle);
        wglMakeCurrent(deviceContextHandle, OGLRenderContext);

        glewExperimental = GL_TRUE;
        glewInit();

        shader.Create("Basic");
        model.Create();
    }

    void Renderer::RenderFrame()
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();

        model.Draw();

        SwapBuffers(GetDC(hWnd));
    }

    void Renderer::Cleanup()
    {
        wglMakeCurrent(GetDC(hWnd), OGLRenderContext);
        wglDeleteContext(OGLRenderContext);
        PostQuitMessage(0);
    }
}