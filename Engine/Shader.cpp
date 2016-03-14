#include "Shader.h"
#include <fstream>
#include <string>

namespace Plasmium
{
    void Shader::Create()
    {
        std::ifstream vs("Assets\\Shaders\\Basic.vs");
        std::ifstream fs("Assets\\Shaders\\Basic.fs");

        const char* vertex_shader = std::string((std::istreambuf_iterator<char>(vs)), std::istreambuf_iterator<char>()).c_str();
        const char* fragment_shader = std::string((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>()).c_str();

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