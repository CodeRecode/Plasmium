#pragma once
#include "GL\glew.h"
#include <gl\GL.h>

namespace Plasmium
{
    class Shader
    {
    private:
        GLuint vertexShader;
        GLuint fragmentShader;
        GLuint program;

    public:
        void Create();
        void Bind();
    };
}