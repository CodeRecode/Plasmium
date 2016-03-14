#include "Renderer.h"
#include <Windows.h>
#include "Types.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Plasmium::Renderer renderer;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

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
        return -1;
    }

    hWnd = CreateWindowEx(NULL, "PSMWindowClass", "Plasmium Engine", WS_OVERLAPPEDWINDOW, 1000, 300, 
        Plasmium::Renderer::WINDOW_WIDTH, Plasmium::Renderer::WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

    if (!hWnd) 
    {
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                break;
            }
        }

        renderer.RenderFrame();
    }

    renderer.Cleanup();

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            renderer.Initialize(hWnd);
            return 0;
        }
        break;
    case WM_DESTROY:
        {
            renderer.Cleanup();
            return 0;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}