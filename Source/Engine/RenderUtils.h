#pragma once
#include "Core.h"

namespace Plasmium {
    struct RenderUtils {
        static HRESULT CreateDeviceAndSwapChain(
            IDXGISwapChain** swapChain,
            ID3D11Device** device,
            ID3D11DeviceContext** deviceContext);

        static HRESULT CreateRenderTargetView(
            IDXGISwapChain* swapChain,
            ID3D11Device* device,
            ID3D11RenderTargetView** renderTargetView);

        static HRESULT CreateDepthBuffers(
            ID3D11Device* device,
            ID3D11DeviceContext* deviceContext,
            ID3D11Texture2D** depthStencilBuffer, 
            ID3D11DepthStencilState** depthStencilState,
            ID3D11DepthStencilState** depthDisabledStencilState,
            ID3D11DepthStencilView** depthStencilView);

        static HRESULT CreateRasterizerStateAndViewport(
            ID3D11Device* device,
            ID3D11DeviceContext* deviceContext,
            ID3D11RasterizerState** rasterState);

        static HRESULT CreateRenderTargetTexture(
            ID3D11Device* device,
            ID3D11Texture2D** renderTargetTexture,
            ID3D11RenderTargetView** renderTargetView,
            ID3D11ShaderResourceView** shaderResourceView);

        static HRESULT CreateBlendStates(
            ID3D11Device* device,
            ID3D11BlendState** blendingStateEnabled,
            ID3D11BlendState** blendingStateDisabled);
    };
}