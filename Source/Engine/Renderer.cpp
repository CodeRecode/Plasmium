#include "Renderer.h"

#include "CameraManager.h"
#include "Core.h"
#include "EntityManager.h"
#include "Plasmath.h"
#include "RenderUtils.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "Types.h"

namespace Plasmium
{
    Renderer::Renderer()
    {
    }

    void Renderer::Initialize()
    {
        HRESULT result;
        result = RenderUtils::CreateDeviceAndSwapChain(&swapChain, &device, &deviceContext);
        if (FAILED(result)) {
            return;
        }

        result = RenderUtils::CreateRenderTargetView(swapChain, device, &renderTargetView);
        if (FAILED(result)) {
            return;
        }

        result = RenderUtils::CreateDepthBuffers(device,
            deviceContext,
            &depthStencilBuffer,
            &depthStencilState,
            &depthDisabledStencilState,
            &depthStencilView);
        if (FAILED(result)) {
            return;
        }

        result = RenderUtils::CreateRasterizerStateAndViewport(device, deviceContext, &rasterState);
        if (FAILED(result)) {
            return;
        }

        result = RenderUtils::CreateBlendStates(device, &blendingStateEnabled, &blendingStateDisabled);
        if (FAILED(result)) {
            return;
        }

        const auto& window = Core::GetWindow();
        float windowHeight = (float)window.GetHeight();
        float windowWidth = (float)window.GetWidth();
        radians fieldOfView = DegreesToRadians(45.0f);
        float aspectRatio = windowWidth / windowHeight;

        projectionMatrix = mat4::ProjectionMatrix(fieldOfView, aspectRatio, screenNear, screenFar);
        orthoMatrix = mat4::OrthogonalMatrix(windowHeight, windowWidth, screenNear, screenFar);

        materialShader.Initialize(device);
        textureShader.Initialize(device);
        spriteShader.Initialize(device);

        fontManager.Initialize(device, swapChain);

        auto& resourceManager = Core::GetResourceManager();
        for (auto& model : resourceManager.GetAllModels()) {
            model.Initialize(device);
        }
        for (auto& texture : resourceManager.GetAllTextures()) {
            texture.Initialize(device);
        }

        sprites.Push(Sprite(FileResource("Assets\\EventLog.png"), rect(10, window.GetHeight() - 110.0f, 550, 100)));
        for (auto& sprite : sprites) {
            sprite.Initialize(device, deviceContext);
        }

        // Add counter debug text to slot 0 and 1
        if (debugTexts.Size() == 0) {
            vec4 drawColor(0.0f, 1.0f, 0.0f, 0.9f);
            rect drawArea((float)window.GetWidth() - 200, 10, 200, 100);
            debugTexts.Push(Text2D("Debug Counter", drawArea, drawColor));
            drawArea.top += 20;
            debugTexts.Push(Text2D("Debug Counter", drawArea, drawColor));
        }
    }

    void Renderer::Update(milliseconds deltaTime)
    {
        deviceContext->OMSetDepthStencilState(depthStencilState, 1);
        deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
        deviceContext->ClearRenderTargetView(renderTargetView, &clearColor[0]);
        deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        auto* camera = Core::GetCameraManager().GetCamera();
        mat4 viewMatrix = camera->GetViewMatrix();

        auto& modelComponents = Core::GetEntityManager().GetComponentArray<ModelComponent>();

        for (const auto& modelComponent : modelComponents) {
            auto* transformComponent = Core::GetEntityManager().GetComponent<TransformComponent>(
                modelComponent.GetId());

            ShaderInternal::MatrixInfoRef matrixInfo(projectionMatrix,
                viewMatrix, 
                transformComponent->GetWorldMatrix());
            if (modelComponent.HasTexture()) {
                textureShader.Bind(deviceContext, matrixInfo);
                modelComponent.Draw(deviceContext, &textureShader);
            }
            else {
                materialShader.Bind(deviceContext, matrixInfo);
                modelComponent.Draw(deviceContext, &materialShader);
            }
        }

        deviceContext->OMSetDepthStencilState(depthDisabledStencilState, 1);
        ShaderInternal::MatrixInfoRef matrixInfo(orthoMatrix, viewMatrix, mat4(1.0f));
        spriteShader.Bind(deviceContext, matrixInfo);
        for (auto& sprite : sprites) {
            sprite.Draw(deviceContext);
        }

        fontManager.Draw(debugTexts);
        fontManager.Draw(gameplayEventLog.GetLogs());

        const auto& window = Core::GetWindow();
        if (window.GetVsyncEnabled()) {
            swapChain->Present(1, 0);
        }
        else {
            swapChain->Present(0, 0);
        }
    }

    void Renderer::Release()
    {
        gameplayEventLog.ClearLog();
        spriteShader.Release();
        textureShader.Release();
        materialShader.Release();

        for (auto& sprite : sprites) {
            sprite.Release();
        }
        auto& resourceManager = Core::GetResourceManager();
        for (auto& model : resourceManager.GetAllModels()) {
            model.Release();
        }
        for (auto& texture : resourceManager.GetAllTextures()) {
            texture.Release();
        }

        if (swapChain != nullptr) {
            swapChain->SetFullscreenState(false, nullptr);
        }
        if (blendingStateDisabled != nullptr) {
            blendingStateDisabled->Release();
            blendingStateDisabled = nullptr;
        }
        if (blendingStateEnabled != nullptr) {
            blendingStateEnabled->Release();
            blendingStateEnabled = nullptr;
        }
        if (depthStencilView != nullptr) {
            depthStencilView->Release();
            depthStencilView = nullptr;
        }
        if (depthDisabledStencilState != nullptr) {
            depthDisabledStencilState->Release();
            depthDisabledStencilState = nullptr;
        }
        if (depthStencilState != nullptr) {
            depthStencilState->Release();
            depthStencilState = nullptr;
        }
        if (depthStencilBuffer != nullptr) {
            depthStencilBuffer->Release();
            depthStencilBuffer = nullptr;
        }
        if (renderTargetView != nullptr) {
            renderTargetView->Release();
            renderTargetView = nullptr;
        }
        if (deviceContext != nullptr) {
            deviceContext->Release();
            deviceContext = nullptr;
        }
        if (device != nullptr) {
            device->Release();
            device = nullptr;
        }
        if (swapChain != nullptr) {
            swapChain->Release();
            swapChain = nullptr;
        }
    }

    void Renderer::ProcessEvent(const GenericEvent& event) {
        auto& resourceManager = Core::GetResourceManager();
        if ((EventType)event.index() == EventType::ModelLoaded) {
            auto& modelEvent = std::get<ModelLoadedEvent>(event);
            auto& model = resourceManager.GetModelResource(modelEvent.file);
            model.Initialize(device);
        }
        if ((EventType)event.index() == EventType::TextureLoaded) {
            auto& textureEvent = std::get<TextureLoadedEvent>(event);
            auto& texture = resourceManager.GetTextureResource(textureEvent.file);
            texture.Initialize(device);
        }
        if ((EventType)event.index() == EventType::PerformanceCounters) {
            auto& performanceCountersEvent = 
                std::get<PerformanceCountersEvent>(event);
            char buffer[256];
            sprintf_s(buffer, "FPS: %.2f", performanceCountersEvent.fps);
            debugTexts[0].ChangeString(buffer);

            sprintf_s(buffer, "CPU: %.2f%%", performanceCountersEvent.cpuPercent);
            debugTexts[1].ChangeString(buffer);
        }
        if ((EventType)event.index() == EventType::GameplayEventLog) {
            auto& gameplayEventLogEvent = std::get<GameplayEventLogEvent>(event);
            gameplayEventLog.AddLog(gameplayEventLogEvent.stringId);
        }
    }
}