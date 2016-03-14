#include "Shader.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew32d.lib")

namespace Plasmium
{
    void Shader::Create()
    {
        const char* vertex_shader =
            "#version 400\n"
            "in vec3 vp;"
            "void main () {"
            "  gl_Position = vec4 (vp, 1.0);"
            "}";

        const char* fragment_shader =
            "#version 400\n"
            "out vec4 frag_colour;"
            "void main () {"
            "  frag_colour = vec4 (1.0, 1.0, 1.0, 1.0);"
            "}";

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertex_shader, nullptr);
        glCompileShader(vertexShader);

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragment_shader, nullptr);
        glCompileShader(fragmentShader);

        program = glCreateProgram();
        glAttachShader(program, fragmentShader);
        glAttachShader(program, vertexShader);
        glLinkProgram(program);
    }

    void Shader::Bind()
    {
        glUseProgram(program);
    }
}