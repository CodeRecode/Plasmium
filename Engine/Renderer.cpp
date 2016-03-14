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

        shader.Create();

        float points[] = {
            0.0f,  0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
            -0.5f, -0.5f,  0.0f
        };

        float colors[] = {
            1.0f,  0.0f,  0.0f,
            0.0f, 1.0f,  0.0f,
            0.0f, 0.0f,  1.0f
        };

        GLuint pointBuffer = 0;
        glGenBuffers(1, &pointBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

        GLuint colorBuffer = 0;
        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STATIC_DRAW);

        vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }

    void Renderer::RenderFrame()
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SwapBuffers(GetDC(hWnd));
    }

    void Renderer::Cleanup()
    {
        wglMakeCurrent(GetDC(hWnd), OGLRenderContext);
        wglDeleteContext(OGLRenderContext);
        PostQuitMessage(0);
    }
}