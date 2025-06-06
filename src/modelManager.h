#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "model.h"
#include "shader.h"
#include "camera.h"

class ModelManager{
    private:
    std::vector<std::unique_ptr<Model>> m_Models;
    std::vector<glm::mat4> m_ModelMats;

    glm::vec3 m_LightPos = glm::vec3(0.0f);
    glm::vec3 m_LightCol = glm::vec3(1.0f);
    float m_Constant = 1.0f;
    float m_Linear = 0.14f;
    float m_Quadratic = 0.07f;

    unsigned int m_Fbo;
    unsigned int m_PickingTexture;
    unsigned int m_DepthTexture;

    public:
    ModelManager();

    void InitFB();
    void Clear();
    void EnablePicking();
    void DisablePicking();
    void DrawPicking(Shader* pickShader);
    
    void AddModel(const std::string& objPath);

    void DrawAll(Camera* camera, Shader* shader);

    void SetupLights(const glm::vec3& lightPos, const glm::vec3& lightCol, float a, float b, float c){
        m_LightPos = lightPos;
        m_LightCol = lightCol;

        m_Constant = a;
        m_Linear = b;
        m_Quadratic = c;
    }

    glm::mat4& GetSelectedModel(){
        return m_ModelMats[selectedModel - 1];
    }
    
    size_t selectedModel = 1;
};

#endif