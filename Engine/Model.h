#pragma once
#include "GL/glew.h"
#include <GL/GL.h>
#include "Shader.h"


namespace Plasmium
{
    class Model
    {
    private:
        GLuint vertexArray;

    public:
        void Create();
        void Draw();
    };
}