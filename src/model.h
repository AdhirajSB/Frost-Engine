#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <GLM/glm.hpp>
#include <vector>
#include <unordered_map>
#include <stb_image/stb_image.h>

#include "shader.h"
#include "camera.h"


struct Vertex{
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 uvCoord;
    glm::vec3 tangent;
};

struct Material{
    float specularExponent = 0.0f;
    
    glm::vec3 ambientColor = glm::vec3(1.0f);
    glm::vec3 diffuseColor = glm::vec3(1.0f);
    glm::vec3 specularColor = glm::vec3(1.0f);
    glm::vec3 emissiveColor = glm::vec3(0.0f);

    float alpha = 1.0f;

    unsigned int diffuseTexture = 0;
    unsigned int normalTexture = 0;
    unsigned int alphaTexture = 0;

    bool hasDiffuseTexture = false;
    bool hasNormalTexture = false;
    bool hasAlphaTexture = false;
};

class Model{
    private:
    std::string m_CurrentMaterial;
    std::unordered_map<std::string, unsigned int> m_RenderID;
    std::unordered_map<std::string, Material> m_MaterialProperty;
    std::unordered_map<std::string, std::vector<Vertex>> m_MaterialGroup;

    void LoadMTL(std::string mtlPath);
    void SetupBuffers();

    public:
    Model(const std::string& objPath);

    void Draw(Camera* camera, Shader* shader);
    void DrawOutline(Shader* outlineShader);
    void DrawPicking() const;
};

#endif