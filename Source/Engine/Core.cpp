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
        coreSystems.Push(&levelManager);
        coreSystems.Push(&resourceManager);
        coreSystems.Push(&cameraManager);
        coreSystems.Push(&entityManager);
        coreSystems.Push(&renderer);

        for (CoreSystem* system : coreSystems) {
            system->Initialize();
        }

        entityManager.RegisterComponentManager(ComponentType::Model, &renderer);
        entityManager.RegisterComponentManager(ComponentType::Transform, &entityManager);
        entityManager.RegisterComponentManager(ComponentType::PlayerController, &entityManager);

        FileResource levelFile = FileResource("Assets\\SampleLevel.lvl");
        levelManager.LoadLevelFile(levelFile);

        while (!window.ShouldQuit())
        {
            milliseconds deltaTime = perfMonitor.FrameStart();
            for (CoreSystem* system : coreSystems) {
                system->Update(deltaTime);
            }
            while (!eventQueue.empty()) {
                auto& event = eventQueue.front();
                for (CoreSystem* system : coreSystems) {
                    system->ProcessEvent(event);
                }
                ProcessEvent(event);
                eventQueue.pop();
            }
            perfMonitor.FrameEnd();
        }

        renderer.Release();
    }

    void Core::PostEvent(GenericEvent&& eventClass)
    {
        eventQueue.push(eventClass);
    }

    void Core::ProcessEvent(const GenericEvent& event)
    {
    }
}