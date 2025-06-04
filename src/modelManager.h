#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "model.h"
#include "shader.h"

class ModelManager{
    private:
    std::vector<std::unique_ptr<Model>> m_Models;
    std::vector<glm::mat4> m_ModelMats;

    unsigned int m_Fbo;
    unsigned int m_PickingTexture;
    unsigned int m_DepthTexture;

    public:
    ModelManager();

    void InitFB();
    void EnablePicking();
    void DisablePicking();
    void DrawPicking(Shader* pickShader);
    
    void AddModel(const std::string& objPath);
    void DrawAll(Shader* shader, Shader* outlineShader);

    size_t selectedModel = 1;
};

#endif