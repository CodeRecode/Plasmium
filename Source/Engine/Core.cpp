#include "Core.h"

#include "Window.h"
#include "Component.h"

#include "Event.h"
#include "InputTypes.h"

namespace Plasmium
{
    void Core::RunGame()
    {
        Window::CreateConsole();
        perfMonitor.Initialize();

        coreSystems.Push(&window);
        coreSystems.Push(&gameplayManager);
        coreSystems.Push(&resourceManager);
        coreSystems.Push(&cameraManager);
        coreSystems.Push(&entityManager);
        coreSystems.Push(&renderer);

        for (CoreSystem* system : coreSystems) {
            system->Initialize();
        }

        entityManager.RegisterComponentManager(ComponentType::Model, &renderer);
        entityManager.RegisterComponentManager(ComponentType::Transform, &entityManager);
        entityManager.RegisterComponentManager(ComponentType::PlayerController, &gameplayManager);
        entityManager.RegisterComponentManager(ComponentType::MonsterController, &gameplayManager);
        entityManager.RegisterComponentManager(ComponentType::Combat, &gameplayManager);
        entityManager.RegisterComponentManager(ComponentType::Name, &gameplayManager);

        FileResource levelFile = FileResource("Assets\\SampleLevel.lvl");
        gameplayManager.LoadLevelFile(levelFile);

        while (!window.ShouldQuit())
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

        renderer.Release();
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
                entityManager.DeleteAllEntities();
                renderer.Release();
                renderer.Initialize();
                gameplayManager.ReloadCurrentLevel();
            }
        }
    }
}