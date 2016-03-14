#include <d3d11.h>
#include <d3dx11.h>
#include <windowsx.h>
#include "D3DMacros.h"
#include "Renderer.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")

namespace Plasmium
{

    bool Renderer::Initialize(HWND hWnd)
    {
        // Initialize D3D
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = WINDOW_WIDTH;
        swapChainDesc.BufferDesc.Height = WINDOW_HEIGHT;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hWnd;
        swapChainDesc.SampleDesc.Count = 4;
        swapChainDesc.Windowed = true;

        HRCheck(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL,
            D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext));

        // Set render target
        ID3D11Texture2D *texBackBuffer = nullptr;
        HRCheck(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texBackBuffer));
        HRCheck(device->CreateRenderTargetView(texBackBuffer, NULL, &backBuffer));

        if (texBackBuffer)
        {
            texBackBuffer->Release();
        }

        deviceContext->OMSetRenderTargets(1, &backBuffer, NULL);

        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = WINDOW_WIDTH;
        viewport.Height = WINDOW_HEIGHT;

        deviceContext->RSSetViewports(1, &viewport);

        return true;
    }

    void Renderer::RenderFrame()
    {
        float clearColor[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
        deviceContext->ClearRenderTargetView(backBuffer, clearColor);

        swapChain->Present(0, 0);
    }

    void Renderer::Cleanup()
    {
        if (backBuffer)
        {
            backBuffer->Release();
        }
        if (swapChain)
        {
            swapChain->Release();
        }
        if (deviceContext)
        {
            deviceContext->Release();
        }
        if (device)
        {
            device->Release();
        }

        backBuffer = nullptr;
        swapChain = nullptr;
        deviceContext = nullptr;
        device = nullptr;
    }
}