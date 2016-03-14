#pragma once
#include "GL\glew.h"
#include <gl\GL.h>
#include <string>

namespace Plasmium
{
    class Shader
    {
    private:
        GLuint program;

        GLint CompileShader(std::string fileType, GLenum type) const;

    public:
        void Create();
        void Bind();
    };
}