#pragma once
#include "CoreSystem.h"
#include "Event.h"
#include "PerfMonitor.h"
#include "RingBuffer.h"


namespace Plasmium
{
    class CameraManager;
    class EntityManager;
    class GameplayManager;
    class Renderer;
    class ResourceManager;
    class Window;

    class Core
    {
    private:
        CameraManager* cameraManager;
        EntityManager* entityManager;
        GameplayManager* gameplayManager;
        Renderer* renderer;
        ResourceManager* resourceManager;
        Window* window;

        PerfMonitor perfMonitor;

        RingBuffer<GenericEvent> eventQueue;
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

        const Window& GetWindow() { return *window; }
        const CameraManager& GetCameraManager() { return *cameraManager; }
        EntityManager& GetEntityManager() { return *entityManager; }
        ResourceManager& GetResourceManager() { return *resourceManager; }

        milliseconds GetFrameStartTime() const { return perfMonitor.GetFrameStartTime(); }
    };
}