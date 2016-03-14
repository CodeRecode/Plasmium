#include "Shader.h"
#include <fstream>
#include <string>

namespace Plasmium
{
    void Shader::Create()
    {
        std::ifstream vs("Assets\\Shaders\\Basic.vs");
        std::ifstream fs("Assets\\Shaders\\Basic.fs");

        std::string vss((std::istreambuf_iterator<char>(vs)), std::istreambuf_iterator<char>());
        std::string fss((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

        const char* vertex_shader = vss.c_str();
        const char* fragment_shader = fss.c_str();

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertex_shader, nullptr);
        glCompileShader(vertexShader);
        int status;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

        if (status != 1)
        {
            int length;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
            char* buffer = new char[length];
            glGetShaderInfoLog(vertexShader, length, NULL, buffer);
            delete buffer;
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragment_shader, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

        if (status != 1)
        {
            int length;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
            char* buffer = new char[length];
            glGetShaderInfoLog(fragmentShader, length, NULL, buffer);
            delete buffer;
        }

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