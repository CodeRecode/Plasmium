#pragma once
#include "Camera.h"
#include "ComponentManager.h"
#include "FontManager.h"
#include "GameplayEventLog.h"
#include "ModelComponent.h"
#include "Shader.h"
#include "Sprite.h"
#include "Text2D.h"
#include "Window.h"

#include "MaterialShader.h"
#include "SpriteShader.h"
#include "TextureShader.h"

#include "mat4.h"
#include "vec4.h"

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

namespace Plasmium
{
    class Renderer : public CoreSystem, public ComponentManager
    {
    private:
        MaterialShader materialShader;
        SpriteShader spriteShader;
        TextureShader textureShader;

        GameplayEventLog gameplayEventLog;
        FontManager fontManager;
        Handler<EntityId, ModelComponent> modelComponents;
        Array<Sprite> sprites;
        Array<Text2D> debugTexts;
        
        vec4 clearColor = vec4(0.05f, 0.05f, 0.05f, 1.0f);
        float screenNear = 0.1f;
        float screenFar = 1000.0f;

        IDXGISwapChain* swapChain = nullptr;
        ID3D11Device* device = nullptr;
        ID3D11DeviceContext* deviceContext = nullptr;
        ID3D11RenderTargetView* renderTargetView = nullptr;

        ID3D11Texture2D* depthStencilBuffer = nullptr;
        ID3D11DepthStencilState* depthStencilState = nullptr;
        ID3D11DepthStencilState* depthDisabledStencilState = nullptr;
        ID3D11DepthStencilView* depthStencilView = nullptr;

        ID3D11Texture2D* shadowRenderTargetTexture = nullptr;
        ID3D11RenderTargetView* shadowRenderTargetView = nullptr;
        ID3D11ShaderResourceView* shadowShaderResourceView = nullptr;

        ID3D11RasterizerState* rasterState = nullptr;

        ID3D11BlendState* blendingStateEnabled = nullptr;
        ID3D11BlendState* blendingStateDisabled = nullptr;

        mat4 projectionMatrix;
        mat4 orthoMatrix;

    public:
        static const D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

        Renderer();

        void Initialize();
        void Update(milliseconds deltaTime) override;
        void Release();

        void ProcessEvent(const GenericEvent& event) override;

        void CreateComponent(const ComponentCreationArgs& creationArgs,
            const FileResource& modelFile) override;
        void CreateComponent(const ComponentCreationArgs& creationArgs,
            const FileResource& modelFile,
            const FileResource& textureFile) override;

        void DeleteComponent(EntityId id, ComponentType type) override;
    };
}