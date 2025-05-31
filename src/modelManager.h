#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "model.h"
#include "shader.h"

class ModelManager{
    private:
    std::vector<std::unique_ptr<Model>> models;
    std::vector<glm::mat4> modelMats;

    public:
    void AddModel(const std::string& objPath);
    void DrawAll(Shader& shader);
};

#endif