#ifndef GRIDLINES_H
#define GRIDLINES_H

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
#include "shader.h"
#include "camera.h"

struct Vec3{
    float x, y, z;
};

class Grid{
    public:
    Grid(float s, float sp): shader("res/shaders/gridVS.glsl", "res/shaders/gridFS.glsl"), VAO(0), VBO(0), size(s), spacing(sp){
        for (float i = -size; i <= size; i += spacing){
            gridLines.push_back({size, 0, i});
            gridLines.push_back({-size, 0, i});

            gridLines.push_back({i, 0, size});
            gridLines.push_back({i, 0, -size});
        }

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, gridLines.size() * sizeof(Vec3), gridLines.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)(0));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        gridLinesSize = gridLines.size();
        gridLines.clear();
    }

    void Draw(const glm::mat4& view, const glm::mat4& projection){
        shader.SetMat4("transform", projection * view);

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, gridLinesSize);
    }

    private:
    Shader shader;
    unsigned int VAO, VBO;
    float size;
    float spacing;
    std::vector<Vec3> gridLines;
    float gridLinesSize;
};

#endif