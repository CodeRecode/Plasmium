#include "Core.h"
#include "Window.h"

namespace Plasmium
{
    void Core::StartGame()
    {
        window = new Window();
        renderer.Initialize();
    }

    void Core::UpdateGame()
    {
        while (!window->ShouldQuit())
        {
            renderer.RenderFrame();
            window->SwapBuffers();
            window->PumpMessages();
        }
    }

    void Core::EndGame()
    {
        renderer.Cleanup();
    }
}