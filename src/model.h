#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <GLM/glm.hpp>
#include <vector>


struct Vertex{
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 uvCoord;
};

class Model{
    private:
    unsigned int m_VAO;
    unsigned int m_VBO;
    int m_vertexCount;

    void SetupBuffers(const std::vector<Vertex>& vertices);

    public:
    Model(const std::string& objPath);
    void Draw() const;
};

#endif