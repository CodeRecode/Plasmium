#pragma once
#include "CameraManager.h"
#include "CoreSystem.h"
#include "EntityManager.h"
#include "Event.h"
#include "GameplayManager.h"
#include "PerfMonitor.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Window.h"

#include <queue>

namespace Plasmium
{
    class Window;
    class Core
    {
    private:
        CameraManager cameraManager;
        EntityManager entityManager;
        GameplayManager gameplayManager;
        PerfMonitor perfMonitor;
        Renderer renderer;
        ResourceManager resourceManager;
        Window window;

        std::queue<GenericEvent> eventQueue;
        Array<DeferredEvent> deferredEvents;
        Array<CoreSystem*> coreSystems;

        Core() { }
    public:
        static Core& GetInstance() { 
            static Core instance;
            return instance;
        }
        void RunGame();

        void PostEvent(GenericEvent&& event);
        void PostDeferredEvent(DeferredEvent&& event);
        void ProcessEvent(const GenericEvent& event);

        const Window& GetWindow() { return window; }
        const CameraManager& GetCameraManager() { return cameraManager; }
        EntityManager& GetEntityManager() { return entityManager; }
        const GameplayManager& GetGameplayManager() { return gameplayManager; }
        ResourceManager& GetResourceManager() { return resourceManager; }

        milliseconds GetFrameStartTime() const { return perfMonitor.GetFrameStartTime(); }
    };
}