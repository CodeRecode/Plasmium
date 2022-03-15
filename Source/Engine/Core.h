#pragma once
#include "CameraManager.h"
#include "CoreSystem.h"
#include "EntityManager.h"
#include "Event.h"
#include "LevelManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include <queue>

namespace Plasmium
{
    class Window;
    class Core
    {
    private:
        uint64 frame = 0;
        uint64 frameStartTime = 0;
        double timeFrequency = 0;
        CameraManager cameraManager;
        EntityManager entityManager;
        LevelManager levelManager;
        Renderer renderer;
        ResourceManager resourceManager;
        Window window;

        std::queue<GenericEvent> eventQueue;
        Array<CoreSystem*> coreSystems;

        Core() { }
    public:
        static Core& GetInstance() { 
            static Core instance;
            return instance;
        }
        void RunGame();

        void PostEvent(GenericEvent&& event);
        void ProcessEvent(const GenericEvent& event);

        const Window& GetWindow() { return window; }
        const CameraManager& GetCameraManager() { return cameraManager; }
        EntityManager& GetEntityManager() { return entityManager; }
        const LevelManager& GetLevelManager() { return levelManager; }
        ResourceManager& GetResourceManager() { return resourceManager; }

        milliseconds GetFrameStartTime() const { return frameStartTime / timeFrequency; }
    };
}