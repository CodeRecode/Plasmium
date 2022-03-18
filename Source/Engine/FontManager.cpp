#include "FontManager.h"

#include "Core.h"
#include "Window.h"
#include <wrl.h>
#include <d2d1_1helper.h>

namespace Plasmium {
    void FontManager::Initialize(ID3D11Device* device, IDXGISwapChain* swapChain)
    {
        HRESULT result;

        result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d1Factory);
        if (FAILED(result)) {
            Window::WriteError("Could not create ID2D1Factory");
            return;
        }

        result = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&writeFactory));
        if (FAILED(result) || writeFactory == nullptr) {
            Window::WriteError("Could not create IDWriteFactory");
            return;
        }

        result = writeFactory->CreateTextFormat(
            L"Courier New",
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            16.0f,
            L"en-us",
            &textFormat
        );
        textFormat->GetFontWeight();
        if (FAILED(result)) {
            Window::WriteError("Could not create IDWriteTextFormat for Consolas");
            return;
        }

        Microsoft::WRL::ComPtr<IDXGISurface> dxgiBackBuffer;
        result = swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
        if (FAILED(result)) {
            Window::WriteError("Could not get swapChain back buffer");
            return;
        }

        const auto& window = Core::GetInstance().GetWindow();
        float dpi = (float)GetDpiForWindow(window.GetHandle());
        D2D1_RENDER_TARGET_PROPERTIES rtProperties =
            D2D1::RenderTargetProperties(
                D2D1_RENDER_TARGET_TYPE_DEFAULT,
                D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
                dpi,
                dpi);

        result = d2d1Factory->CreateDxgiSurfaceRenderTarget(dxgiBackBuffer.Get(), rtProperties, &renderTarget);
        dxgiBackBuffer->Release();
        if (FAILED(result)) {
            Window::WriteError("Could not create ID2D1RenderTarget");
            return;
        }
        renderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

        result = renderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Green),
            &fontBrush
        );
        if (FAILED(result)) {
            Window::WriteError("Could not create ID2D1SolidColorBrush");
            return;
        }
    }

    void FontManager::Draw(const std::deque<Text2D>& texts)
    {
        const auto& window = Core::GetInstance().GetWindow();
        renderTarget->BeginDraw();

        for (const auto& text : texts) {
            const rect& area = text.GetDrawArea();
            D2D1_RECT_F layoutRect = D2D1::RectF(
                area.x,
                area.y,
                area.x + area.width,
                area.y + area.height);

            fontBrush->SetColor((const D3DCOLORVALUE*)&text.GetDrawColor());
            renderTarget->DrawText(
                text.GetString(),
                text.GetLength(),
                textFormat,
                layoutRect,
                fontBrush
            );
        }

        renderTarget->EndDraw();
    }

    void FontManager::Draw(const Array<Text2D>& texts)
    {
        const auto& window = Core::GetInstance().GetWindow();
        renderTarget->BeginDraw();

        for (const auto& text : texts) {
            const rect& area = text.GetDrawArea();
            D2D1_RECT_F layoutRect = D2D1::RectF(
                area.x,
                area.y,
                area.x + area.width,
                area.y + area.height);

            fontBrush->SetColor((const D3DCOLORVALUE *)&text.GetDrawColor());
            renderTarget->DrawText(
                text.GetString(),
                text.GetLength(),
                textFormat,
                layoutRect,
                fontBrush
            );
        }

        renderTarget->EndDraw();
    }

    void FontManager::Release()
    {
        fontBrush->Release();
        renderTarget->Release();
        textFormat->Release();
        writeFactory->Release();
        d2d1Factory->Release();
    }
}