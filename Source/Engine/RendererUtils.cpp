#include "RenderUtils.h"

#include "Renderer.h"
#include "Window.h"

namespace Plasmium {

    DXGI_RATIONAL GetRefreshRate(const Window& window)
    {
        DXGI_RATIONAL defaultRate;
        defaultRate.Numerator = 0;
        defaultRate.Denominator = 1;

        HRESULT result;
        IDXGIFactory* factory;
        result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
        if (FAILED(result)) {
            Window::WriteError("Could not create IDXGIFactory");
            return defaultRate;
        }

        IDXGIAdapter* adapter;
        result = factory->EnumAdapters(0, &adapter);
        if (FAILED(result)) {
            Window::WriteError("Could not enumerate adapters");
            return defaultRate;
        }

        IDXGIOutput* adapterOutput;
        result = adapter->EnumOutputs(0, &adapterOutput);
        if (FAILED(result)) {
            Window::WriteError("Could not enumerate adapter outputs");
            return defaultRate;
        }

        uint32 numModes = 0;
        result = adapterOutput->GetDisplayModeList(
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_ENUM_MODES_INTERLACED,
            &numModes,
            nullptr);
        if (FAILED(result) || numModes == 0) {
            Window::WriteError("Could not get display mode list or there were none");
            return defaultRate;
        }

        DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
        result = adapterOutput->GetDisplayModeList(
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_ENUM_MODES_INTERLACED,
            &numModes,
            displayModeList);
        if (FAILED(result)) {
            Window::WriteError("Could not get display mode list");
            delete[] displayModeList;
            return defaultRate;
        }

        DXGI_MODE_DESC* windowDisplayMode = nullptr;
        for (uint32 mode = 0; mode < numModes; ++mode) {
            if (displayModeList[mode].Width == window.GetWidth() &&
                displayModeList[mode].Height == window.GetHeight()) {
                windowDisplayMode = &displayModeList[mode];
            }
        }
        if (windowDisplayMode == nullptr) {
            Window::WriteError("Could not get display mode for the window");
            delete[] displayModeList;
            return defaultRate;
        }

        DXGI_RATIONAL refreshRate = windowDisplayMode->RefreshRate;

        delete[] displayModeList;
        adapterOutput->Release();
        adapter->Release();
        factory->Release();

        return refreshRate;
    }

    HRESULT RenderUtils::CreateDeviceAndSwapChain(
        IDXGISwapChain** swapChain,
        ID3D11Device** device,
        ID3D11DeviceContext** deviceContext)
    {
        HRESULT result;
        const auto& window = Core::GetInstance().GetWindow();

        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Height = window.GetHeight();
        swapChainDesc.BufferDesc.Width = window.GetWidth();
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        if (window.GetVsyncEnabled()) {
            swapChainDesc.BufferDesc.RefreshRate = GetRefreshRate(window);
        }
        else {
            swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
            swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        }

        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = window.GetHandle();

        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        swapChainDesc.Windowed = window.GetWindowed();

        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = 0;

        result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT,
            &Renderer::FeatureLevel,
            1,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            swapChain,
            device,
            nullptr,
            deviceContext);
        if (FAILED(result)) {
            Window::WriteError("Call to D3D11CreateDeviceAndSwapChain failed");
            return result;
        }

        return result;
    }

    HRESULT RenderUtils::CreateRenderTargetView(
        IDXGISwapChain* swapChain,
        ID3D11Device* device,
        ID3D11RenderTargetView** renderTargetView)
    {
        HRESULT result;
        ID3D11Texture2D* backBufferPtr;
        result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferPtr);
        if (FAILED(result)) {
            Window::WriteError("Call to swapChain->GetBuffer failed");
            return result;
        }

        result = device->CreateRenderTargetView(backBufferPtr, nullptr, renderTargetView);
        if (FAILED(result)) {
            Window::WriteError("Call to CreateRenderTargetView failed");
            return result;
        }
        backBufferPtr->Release();
        
        return result;
    }

    HRESULT RenderUtils::CreateDepthBuffers(
        ID3D11Device* device,
        ID3D11DeviceContext* deviceContext,
        ID3D11Texture2D** depthStencilBuffer,
        ID3D11DepthStencilState** depthStencilState,
        ID3D11DepthStencilState** depthDisabledStencilState,
        ID3D11DepthStencilView** depthStencilView)
    {
        HRESULT result;
        const auto& window = Core::GetInstance().GetWindow();

        D3D11_TEXTURE2D_DESC depthBufferDesc;
        ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

        depthBufferDesc.Height = window.GetHeight();
        depthBufferDesc.Width = window.GetWidth();
        depthBufferDesc.MipLevels = 1;
        depthBufferDesc.ArraySize = 1;
        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count = 1;
        depthBufferDesc.SampleDesc.Quality = 0;
        depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthBufferDesc.CPUAccessFlags = 0;
        depthBufferDesc.MiscFlags = 0;

        result = device->CreateTexture2D(&depthBufferDesc, nullptr, depthStencilBuffer);
        if (FAILED(result)) {
            Window::WriteError("Call to create depth stencil buffer failed");
            return result;
        }

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        depthStencilDesc.StencilEnable = true;
        depthStencilDesc.StencilReadMask = 0xFF;
        depthStencilDesc.StencilWriteMask = 0xFF;

        depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        result = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState);
        if (FAILED(result)) {
            Window::WriteError("Call to create depth stencil state failed");
            return result;
        }

        D3D11_DEPTH_STENCIL_DESC depthDisableStencilDesc;
        ZeroMemory(&depthDisableStencilDesc, sizeof(depthDisableStencilDesc));

        depthDisableStencilDesc.DepthEnable = false;
        depthDisableStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthDisableStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        depthDisableStencilDesc.StencilEnable = true;
        depthDisableStencilDesc.StencilReadMask = 0xFF;
        depthDisableStencilDesc.StencilWriteMask = 0xFF;

        depthDisableStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthDisableStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthDisableStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthDisableStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        depthDisableStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthDisableStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthDisableStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthDisableStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        result = device->CreateDepthStencilState(&depthDisableStencilDesc, depthDisabledStencilState);
        if (FAILED(result)) {
            Window::WriteError("Call to create depth disabled stencil state failed");
            return result;
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

        depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;

        result = device->CreateDepthStencilView(*depthStencilBuffer, &depthStencilViewDesc, depthStencilView);
        if (FAILED(result)) {
            Window::WriteError("Call to create depth stencil view failed");
            return result;
        }

        return result;
    }

    HRESULT RenderUtils::CreateRasterizerStateAndViewport(
        ID3D11Device* device,
        ID3D11DeviceContext* deviceContext,
        ID3D11RasterizerState** rasterState)
    {
        HRESULT result;
        const auto& window = Core::GetInstance().GetWindow();

        D3D11_RASTERIZER_DESC rasterDesc;
        ZeroMemory(&rasterDesc, sizeof(rasterDesc));
        rasterDesc.AntialiasedLineEnable = false;
        rasterDesc.CullMode = D3D11_CULL_BACK;
        rasterDesc.DepthBias = 0;
        rasterDesc.DepthBiasClamp = 0.0f;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.MultisampleEnable = false;
        rasterDesc.ScissorEnable = false;
        rasterDesc.SlopeScaledDepthBias = 0.0f;

        result = device->CreateRasterizerState(&rasterDesc, rasterState);
        if (FAILED(result)) {
            Window::WriteError("Could not create rasterizer state");
            return result;
        }

        deviceContext->RSSetState(*rasterState);

        D3D11_VIEWPORT viewport;
        viewport.Height = (float)window.GetHeight();
        viewport.Width = (float)window.GetWidth();
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;

        deviceContext->RSSetViewports(1, &viewport);

        return result;
    }

    HRESULT RenderUtils::CreateRenderTargetTexture(
        ID3D11Device* device, 
        ID3D11Texture2D** renderTargetTexture,
        ID3D11RenderTargetView** renderTargetView,
        ID3D11ShaderResourceView** shaderResourceView)
    {
        HRESULT result;
        const auto& window = Core::GetInstance().GetWindow();

        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        textureDesc.Height = window.GetHeight();
        textureDesc.Width = window.GetWidth();
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        result = device->CreateTexture2D(&textureDesc, nullptr, renderTargetTexture);
        if (FAILED(result)) {
            Window::WriteError("Could not create 2D renderable texture");
            return result;
        }

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;

        result = device->CreateRenderTargetView(*renderTargetTexture, &renderTargetViewDesc, renderTargetView);
        if (FAILED(result)) {
            Window::WriteError("Could not create render target view");
            return result;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
        ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
        shaderResourceViewDesc.Format = textureDesc.Format;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;

        result = device->CreateShaderResourceView(*renderTargetTexture, &shaderResourceViewDesc, shaderResourceView);
        if (FAILED(result)) {
            Window::WriteError("Could not create render target view");
            return result;
        }

        return result;
    }

    HRESULT RenderUtils::CreateBlendStates(
        ID3D11Device* device,
        ID3D11BlendState** blendingStateEnabled,
        ID3D11BlendState** blendingStateDisabled)
    {
        HRESULT result;
        D3D11_BLEND_DESC blendDesc;
        ZeroMemory(&blendDesc, sizeof(blendDesc));

        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

        result = device->CreateBlendState(&blendDesc, blendingStateEnabled);
        if (FAILED(result)) {
            Window::WriteError("Could not create blend state");
            return result;
        }

        blendDesc.RenderTarget[0].BlendEnable = false;

        result = device->CreateBlendState(&blendDesc, blendingStateDisabled);
        if (FAILED(result)) {
            Window::WriteError("Could not create blend state");
            return result;
        }

        return result;
    }
}
