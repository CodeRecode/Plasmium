#include "Core.h"

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
        perfMonitor.Initialize();

        cameraManager = new CameraManager();
        entityManager = new EntityManager();
        gameplayManager = new GameplayManager();
        renderer = new Renderer();
        resourceManager = new ResourceManager();
        window = new Window();

        coreSystems.Push(window);
        coreSystems.Push(gameplayManager);
        coreSystems.Push(resourceManager);
        coreSystems.Push(cameraManager);
        coreSystems.Push(entityManager);
        coreSystems.Push(renderer);

        for (CoreSystem* system : coreSystems) {
            system->Initialize();
        }

        FileResource levelFile = FileResource("Assets\\SampleLevel.lvl");
        gameplayManager->LoadLevelFile(levelFile);

        while (!eventQueue.Empty()) {
            auto event = eventQueue.PopFront();
            for (CoreSystem* system : coreSystems) {
                system->ProcessEvent(event);
            }
            ProcessEvent(event);
        }

        while (!window->ShouldQuit())
        {
            milliseconds deltaTime = perfMonitor.FrameStart();
            for (CoreSystem* system : coreSystems) {
                system->Update(deltaTime);
            }

            milliseconds frameStartTime = perfMonitor.GetFrameStartTime();
            for (uint32 index = 0; index < deferredEvents.Size(); ++index) {
                if (frameStartTime >= deferredEvents[index].eventTime) {
                    PostEvent(std::move(deferredEvents[index].event));
                    deferredEvents.Delete(index);
                    --index;
                }
            }

            while (!eventQueue.Empty()) {
                auto event = eventQueue.PopFront();
                for (CoreSystem* system : coreSystems) {
                    system->ProcessEvent(event);
                }
                ProcessEvent(event);
            }

            perfMonitor.FrameEnd();
        }

        for (CoreSystem* system : coreSystems) {
            system->Release();
        }
    }

    void Core::PostDeferredEvent(DeferredEvent&& event)
    {
        deferredEvents.Push(std::move(event));
    }

    void Core::PostEvent(GenericEvent&& eventClass)
    {
        eventQueue.PushBack(std::move(eventClass));
    }

    void Core::ProcessEvent(const GenericEvent& event)
    {
        if ((EventType)event.index() == EventType::Input) {
            auto& inputEvent = std::get<InputEvent>(event);

            if (inputEvent.GetKeyToggleDown(KeybindFunction::ReloadLevel)) {
                eventQueue.Clear();
                deferredEvents.Clear();

                // Tear down immediately in a safe order
                entityManager->DeleteAllEntities();
                renderer->Release();
                renderer->Initialize();
                gameplayManager->ReloadCurrentLevel();
            }
        }
    }
}