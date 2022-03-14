#include "Renderer.h"

#include "Core.h"
#include "RenderUtils.h"
#include "Types.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Plasmium
{
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

        const auto& window = Core::GetInstance().GetWindow();
        float windowHeight = (float)window.GetHeight();
        float windowWidth = (float)window.GetWidth();
        radians fieldOfView = (radians)M_PI_4; // 90 degrees
        float aspectRatio = windowWidth / windowHeight;

        projectionMatrix = mat4::ProjectionMatrix(fieldOfView, aspectRatio, screenNear, screenFar);
        orthoMatrix = mat4::OrthogonalMatrix(windowHeight, windowWidth, screenNear, screenFar);

        materialShader.Initialize(device);
        textureShader.Initialize(device);
        spriteShader.Initialize(device);

        sprite = new Sprite(FileResource("Assets\\gobling.jpg"), rect(10,10,50,50));
        sprite->Initialize(device, deviceContext);
        fontManager.Initialize(device, swapChain);

        auto& resourceManager = Core::GetInstance().GetResourceManager();
        for (auto& model : resourceManager.GetAllModels()) {
            model.Initialize(device);
        }
        for (auto& texture : resourceManager.GetAllTextures()) {
            texture.Initialize(device);
        }
    }

    void Renderer::Update()
    {
        deviceContext->OMSetDepthStencilState(depthStencilState, 1);
        deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
        deviceContext->ClearRenderTargetView(renderTargetView, &clearColor[0]);
        deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        auto& entityManager = Core::GetInstance().GetEntityManager();

        auto& camera = Core::GetInstance().GetCameraManager().GetCamera();
        for (const auto& modelComponent : modelComponents.GetObjectsReference()) {
            auto* transformComponent = entityManager.GetTransform(modelComponent.GetId());

            auto worldMatrix = mat4(1.0f);
            worldMatrix = worldMatrix.Translate(transformComponent->GetPosition());
            worldMatrix = worldMatrix.Rotate(transformComponent->GetRotation());
            worldMatrix = worldMatrix.Scale(transformComponent->GetScale());

            ShaderInternal::MatrixInfo matrixInfo(projectionMatrix, camera.GetViewMatrix(), worldMatrix);
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
        ShaderInternal::MatrixInfo matrixInfo(orthoMatrix, camera.GetViewMatrix(), mat4(1.0f));
        spriteShader.Bind(deviceContext, matrixInfo);
        sprite->Draw(deviceContext);

        fontManager.Draw();

        const auto& window = Core::GetInstance().GetWindow();
        if (window.GetVsyncEnabled()) {
            swapChain->Present(1, 0);
        }
        else {
            swapChain->Present(0, 0);
        }
    }

    void Renderer::Release()
    {
        spriteShader.Release();
        textureShader.Release();
        materialShader.Release();

        auto& resourceManager = Core::GetInstance().GetResourceManager();
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
        auto& resourceManager = Core::GetInstance().GetResourceManager();
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
        if ((EventType)event.index() == EventType::Input) {
            auto& inputEvent = std::get<InputEvent>(event);
            if (inputEvent.GetKeyDown(InputKey::F9)) {
                Release();
                Initialize();
            }
        }
    }

    void Renderer::CreateComponent(const ComponentCreationArgs& creationArgs,
        const FileResource& modelFile)
    {
        assert(creationArgs.type == ComponentType::Model);
        auto& resourceManager = Core::GetInstance().GetResourceManager();

        resourceManager.GetModelResource(modelFile);
        modelComponents.EmplaceObject(creationArgs.parent, ModelComponent(creationArgs, modelFile));
    }

    void Renderer::CreateComponent(const ComponentCreationArgs& creationArgs,
        const FileResource& modelFile,
        const FileResource& textureFile)
    {
        assert(creationArgs.type == ComponentType::Model);
        auto& resourceManager = Core::GetInstance().GetResourceManager();

        resourceManager.GetModelResource(modelFile);
        resourceManager.GetTextureResource(textureFile);
        modelComponents.EmplaceObject(creationArgs.parent, ModelComponent(creationArgs, modelFile, textureFile));
    }

    void Renderer::DeleteComponent(EntityId id, ComponentType type)
    {
        assert(type == ComponentType::Model);
        modelComponents.DeleteObject(id);
    }
}