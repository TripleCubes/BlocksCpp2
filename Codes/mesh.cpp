#include "mesh.h"

void Mesh::set(std::vector<float> verticies)
{
    if (VAOInitialized)
    {
        glDeleteVertexArrays(1, &VAO);
    }
    else
    {
        VAOInitialized = true;
    }

    if (EBOInitialized)
    {
        glDeleteVertexArrays(1, &EBO);
        EBOInitialized = false;
    }

    numberOfVerticies = verticies.size() / 8;

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
}

void Mesh::set(std::vector<float> verticies, std::vector<unsigned int> indicies)
{
    if (VAOInitialized)
    {
        glDeleteVertexArrays(1, &VAO);
    }
    else
    {
        VAOInitialized = true;
    }

    if (EBOInitialized)
    {
        glDeleteVertexArrays(1, &EBO);
    }
    else
    {
        EBOInitialized = true;
    }

    numberOfIndicies = indicies.size();

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(float), &indicies[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
}

void Mesh::set2d(std::vector<float> verticies)
{
    if (VAOInitialized)
    {
        glDeleteVertexArrays(1, &VAO);
    }
    else
    {
        VAOInitialized = true;
    }

    if (EBOInitialized)
    {
        glDeleteVertexArrays(1, &EBO);
        EBOInitialized = false;
    }

    numberOfVerticies = verticies.size() / 2;

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
}

void Mesh::set2d(std::vector<float> verticies, std::vector<unsigned int> indicies)
{
    if (VAOInitialized)
    {
        glDeleteVertexArrays(1, &VAO);
    }
    else
    {
        VAOInitialized = true;
    }

    if (EBOInitialized)
    {
        glDeleteVertexArrays(1, &EBO);
    }
    else
    {
        EBOInitialized = true;
    }

    numberOfIndicies = indicies.size();

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(float), &indicies[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
}

void Mesh::draw(bool drawLine)
{
    glBindVertexArray(VAO);
    if (!EBOInitialized)
    {
        glDrawArrays(GL_TRIANGLES, 0, numberOfVerticies);
    }
    else 
    {
        glLineWidth(2);
        glDrawElements(drawLine ? GL_LINES : GL_TRIANGLES, numberOfIndicies, GL_UNSIGNED_INT, 0);
    }
}

void Mesh::release()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
}