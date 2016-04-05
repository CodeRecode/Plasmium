#include "Types.h"
#include "Renderer.h"

namespace Plasmium
{
    void Renderer::Initialize()
    {
        shader.Create("Basic");
        model.Create();
    }

    void Renderer::RenderFrame()
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();

        model.Draw();
    }

    void Renderer::Cleanup()
    {
    }
}