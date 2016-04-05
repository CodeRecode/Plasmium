#include "Window.h"
#include <Windows.h>
#include "GL/glew.h"
#include <GL/GL.h>

namespace Plasmium
{
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
        {
            return 0;
        }
        break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_QUIT:
        {
            return 0;
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    Window::Window() : shouldQuit(false)
    {
        WNDCLASSEX wc;
        HINSTANCE hInstance = GetModuleHandle(NULL);

        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = "PSMWindowClass";

        if (!RegisterClassEx(&wc))
        {
            // Fail
        }

        handle = CreateWindowEx(NULL, "PSMWindowClass", "Plasmium Engine", WS_OVERLAPPEDWINDOW, 1000, 300,
            WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);


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

        HDC deviceContextHandle = GetDC(handle);

        int32 pixelFormat = ChoosePixelFormat(deviceContextHandle, &pixelFormatDesc);
        SetPixelFormat(deviceContextHandle, pixelFormat, &pixelFormatDesc);

        OGLRenderContext = wglCreateContext(deviceContextHandle);
        wglMakeCurrent(deviceContextHandle, OGLRenderContext);

        glewExperimental = GL_TRUE;
        glewInit();

        ShowWindow(handle, SW_SHOWDEFAULT);
    }

    Window::~Window()
    {
        wglMakeCurrent(GetDC(handle), OGLRenderContext);
        wglDeleteContext(OGLRenderContext);
    }


    void Window::PumpMessages()
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                shouldQuit = true;
            }
        }
    }

    void Window::SwapBuffers()
    {
        ::SwapBuffers(GetDC(handle));
    }
}
