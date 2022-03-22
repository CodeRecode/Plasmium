#include "Core.h"

#include "AnimationManager.h"
#include "CameraManager.h"
#include "Component.h"
#include "EntityManager.h"
#include "Event.h"
#include "GameplayManager.h"
#include "InputTypes.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Window.h"


namespace Plasmium
{
    void Core::RunGame()
    {
        Window::CreateConsole();
        timer.Initialize();

        animationManager = std::make_shared<AnimationManager>();
        cameraManager = std::make_shared<CameraManager>();
        entityManager = std::make_shared<EntityManager>();
        gameplayManager = std::make_shared<GameplayManager>();
        renderer = std::make_shared<Renderer>();
        resourceManager = std::make_shared<ResourceManager>();
        window = std::make_shared<Window>();

        coreSystems.Push(window);
        coreSystems.Push(gameplayManager);
        coreSystems.Push(animationManager);
        coreSystems.Push(resourceManager);
        coreSystems.Push(cameraManager);
        coreSystems.Push(entityManager);
        coreSystems.Push(renderer);

        for (auto& system : coreSystems) {
            system->Initialize();
        }

        FileResource levelFile = FileResource("Configs\\Levels\\ProcLevel.plvl");
        gameplayManager->LoadLevelFile(levelFile);

        ProcessAllEvents();

        while (!window->ShouldQuit())
        {
            milliseconds deltaTime = timer.FrameStart();
            for (auto& system : coreSystems) {
                system->Update(deltaTime);
            }

            ProcessAllEvents();

            timer.FrameEnd();
        }

        for (auto& system : coreSystems) {
            system->Release();
        }
    }

    void Core::PostDeferredEvent(DeferredEvent&& event)
    {
        GetInstance().deferredEvents.Push(std::move(event));
    }

    void Core::PostEvent(GenericEvent&& eventClass)
    {
        GetInstance().eventQueue.PushBack(std::move(eventClass));
    }

    void Core::ProcessAllEvents()
    {
        milliseconds frameStartTime = timer.GetFrameStartTime();
        for (uint32 index = 0; index < deferredEvents.Size(); ++index) {
            if (frameStartTime >= deferredEvents[index].eventTime) {
                PostEvent(std::move(deferredEvents[index].event));
                deferredEvents.Delete(index);
                --index;
            }
        }

        while (!eventQueue.Empty()) {
            auto event = eventQueue.PopFront();
            for (auto& system : coreSystems) {
                system->ProcessEvent(event);
            }
            ProcessEvent(event);
        }
    }

    void Core::ProcessEvent(const GenericEvent& event)
    {
        if ((EventType)event.index() == EventType::Input) {
            auto& inputEvent = std::get<InputEvent>(event);

            if (inputEvent.GetKeyToggleDown(KeybindFunction::ReloadLevel)) {
                eventQueue.Clear();
                deferredEvents.Clear();

                // Tear down immediately in a safe order
                animationManager->StopAll();
                entityManager->DeleteAllEntities();
                renderer->Release();
                renderer->Initialize();
                gameplayManager->ReloadCurrentLevel();
            }
        }
    }
}