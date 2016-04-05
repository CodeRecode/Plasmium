#pragma once
#include "Renderer.h"

namespace Plasmium
{
    class Window;
    class Core
    {
    private:
        Window* window;
        Renderer renderer;

    public:
        void StartGame();
        void UpdateGame();
        void EndGame();
    };
}