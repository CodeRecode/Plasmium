#pragma once
#include <Windows.h>

class IDXGISwapChain;
class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11RenderTargetView;

namespace Plasmium
{
    class Renderer
    {
    private:
        IDXGISwapChain *swapChain;
        ID3D11Device *device;
        ID3D11DeviceContext *deviceContext;
        ID3D11RenderTargetView *backBuffer;

    public:
        const static int WINDOW_WIDTH = 500;
        const static int WINDOW_HEIGHT = 400;

        bool Initialize(HWND hWnd);
        void RenderFrame();
        void Cleanup();
    };
}