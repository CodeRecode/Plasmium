#pragma once
#include "Shader.h"
#include "Model.h"

namespace Plasmium
{
    class Renderer
    {
    private:
        Shader shader; 
        Model model;

    public:
        void Initialize();
        void RenderFrame();
        void Cleanup();
    };
}