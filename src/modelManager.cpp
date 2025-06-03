#include "modelManager.h"

ModelManager::ModelManager(){
    AddModel("res/sampleModels/Cube/cube.obj");
}

void ModelManager::AddModel(const std::string &objPath)
{
    models.push_back(std::make_unique<Model>(objPath));
    modelMats.push_back(glm::mat4(1.0f));
}

void ModelManager::DrawAll(Shader& shader){
    for (size_t i = 0; i < models.size(); i++){
        shader.SetMat4("model", modelMats[i]);
        models[i]->Draw();
    }
}
