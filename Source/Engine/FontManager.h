#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include "Array.h"
#include "Text2D.h"
#include "Types.h"
#include <deque>

namespace Plasmium {
    class FontManager {
    private:
        IDWriteFactory* writeFactory;
        ID2D1Factory1* d2d1Factory;

        IDWriteTextFormat* textFormat;
        ID2D1RenderTarget* renderTarget;
        ID2D1SolidColorBrush* fontBrush;

    public:
        void Initialize(ID3D11Device* device, IDXGISwapChain* swapChain);
        void Draw(const std::deque<Text2D>& texts);
        void Draw(const Array<Text2D>& texts);
        void Release();
    };
}