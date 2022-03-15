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

        uint64 lastPerfTime = 0, lastPerfFrame = 0, lastFrameTime = 0, frequency = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&lastPerfTime);
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
        timeFrequency = frequency / 1000.0f; // frequency is in seconds

        lastFrameTime = lastPerfTime;
        while (!window.ShouldQuit())
        {
            QueryPerformanceCounter((LARGE_INTEGER*)&frameStartTime);
            milliseconds deltaTime = (frameStartTime - lastFrameTime) / timeFrequency;
            Window::WriteError(std::to_string(deltaTime));
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
            ++frame;
            lastFrameTime = frameStartTime;

            milliseconds timeDelta = (frameStartTime - lastPerfTime) / (timeFrequency);
            if (timeDelta > 100) {
                PostEvent(PerformanceCountersEvent(frame,
                    frameStartTime / timeFrequency,
                    (float)(frame - lastPerfFrame) / (float)(timeDelta / 1000.0f),
                    1.0f));

                lastPerfTime = frameStartTime;
                lastPerfFrame = frame;
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