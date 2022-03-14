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

        uint64 startTime = 0, currentTime = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

        while (!window.ShouldQuit())
        {
            for (CoreSystem* system : coreSystems) {
                system->Update();
            }
            while (!eventQueue.empty()) {
                auto& event = eventQueue.front();
                for (CoreSystem* system : coreSystems) {
                    system->ProcessEvent(event);
                }
                ProcessEvent(event);
                eventQueue.pop();
            }
            ++frame;
            if (frame % 60 == 0) {
                QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
                double timeDiff = (double)(currentTime - startTime) / 10000000.0;
                /*Window::WriteError(std::to_string(frame) + " : " + 
                    std::to_string(timeDiff) + " : " +
                    std::to_string((double)frame / timeDiff));*/
            }
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