#include "Model.h"
namespace Plasmium
{
    void Model::Create()
    {
        float points[] = {
            0.0f,  0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
            -0.5f, -0.5f,  0.0f
        };

        float colors[] = {
            1.0f,  0.0f,  0.0f,
            0.0f, 1.0f,  0.0f,
            0.0f, 0.0f,  1.0f
        };

        GLuint pointBuffer = 0;
        glGenBuffers(1, &pointBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

        GLuint colorBuffer = 0;
        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STATIC_DRAW);

        vertexArray = 0;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }

    void Model::Draw()
    {
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}