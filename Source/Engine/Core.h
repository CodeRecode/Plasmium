#pragma once
#include "CoreSystem.h"
#include "Event.h"
#include "Timer.h"
#include "RingBuffer.h"

namespace Plasmium
{
    class AnimationManager;
    class CameraManager;
    class EntityManager;
    class GameplayManager;
    class Renderer;
    class ResourceManager;
    class Window;

    class Core
    {
    private:
        std::shared_ptr<AnimationManager> animationManager;
        std::shared_ptr<CameraManager> cameraManager;
        std::shared_ptr<EntityManager> entityManager;
        std::shared_ptr<GameplayManager> gameplayManager;
        std::shared_ptr<Renderer> renderer;
        std::shared_ptr<ResourceManager> resourceManager;
        std::shared_ptr<Window> window;

        Timer timer;

        RingBuffer<GenericEvent> eventQueue;
        Array<DeferredEvent> deferredEvents;
        Array<std::shared_ptr<CoreSystem>> coreSystems;

        Core() = default;
        ~Core() = default;
        void ProcessAllEvents();
        void ProcessEvent(const GenericEvent& event);

    public:
        void RunGame();

        static Core& GetInstance() {
            static Core instance;
            return instance;
        }

        static void PostEvent(GenericEvent&& event);
        static void PostDeferredEvent(DeferredEvent&& event);

        static const Window& GetWindow() { return *GetInstance().window; }
        static const CameraManager& GetCameraManager() { return *GetInstance().cameraManager; }
        static EntityManager& GetEntityManager() { return *GetInstance().entityManager; }
        static ResourceManager& GetResourceManager() { return *GetInstance().resourceManager; }

        static uint32 GetNextRandom() { return GetInstance().timer.GetNextRandom(); }
        static milliseconds GetFrameStartTime() { return GetInstance().timer.GetFrameStartTime(); }
    };
}