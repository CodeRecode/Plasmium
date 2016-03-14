#include "Shader.h"
#include "Types.h"
#include <fstream>

namespace Plasmium
{

    GLint Shader::CompileShader(std::string fileType, GLenum type) const
    {
        std::ifstream fileStream("Assets\\Shaders\\Basic" + fileType);
        std::string shaderString((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
        const char* shaderSource = shaderString.c_str();

        GLint shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);

        int32 status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status != 1)
        {
            int32 length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            char* buffer = new char[length];
            glGetShaderInfoLog(shader, length, NULL, buffer);
            printf("ERROR: Shader %s compiled unsuccessfully: \n%s\n", fileType.c_str(), buffer);
            delete buffer;

            return 0;
        }

        return shader;
    }

    void Shader::Create()
    {
        GLuint vertexShader = CompileShader(".vs", GL_VERTEX_SHADER);
        GLuint fragmentShader = CompileShader(".fs", GL_FRAGMENT_SHADER);

        program = glCreateProgram();

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        glDetachShader(program, vertexShader);
        glDeleteShader(vertexShader);

        glDetachShader(program, fragmentShader);
        glDeleteShader(fragmentShader);
    }

    void Shader::Bind()
    {
        glUseProgram(program);
    }
}