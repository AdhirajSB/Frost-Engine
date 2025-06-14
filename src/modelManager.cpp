#include "modelManager.h"

ModelManager::ModelManager(){
    AddModel("res/primitives/cube.obj");
    InitFB();
}

void ModelManager::InitFB(){
    glGenFramebuffers(1, &m_Fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
    
    glGenTextures(1, &m_PickingTexture);
    glBindTexture(GL_TEXTURE_2D, m_PickingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, 3840, 2400, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PickingTexture, 0);

    glGenTextures(1, &m_DepthTexture);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 3840, 2400, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete!" << std::endl;
    }


    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModelManager::Clear(){
    EnablePicking();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    DisablePicking();
}

void ModelManager::EnablePicking(){
    glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
}

void ModelManager::DisablePicking(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModelManager::DrawPicking(Shader* pickShader){
    EnablePicking();
    for (size_t i = 0; i < m_Models.size(); i++){
        pickShader->SetMat4("model", m_ModelMats[i]);
        pickShader->SetUInt("objectID", static_cast<unsigned int>(i + 1));

        m_Models[i]->DrawPicking();
    }
    DisablePicking();
}

void ModelManager::AddModel(const std::string &objPath){
    m_Models.push_back(std::make_unique<Model>(objPath));
    m_ModelMats.push_back(glm::mat4(1.0f));
}

void ModelManager::DrawAll(Camera* camera, Shader* shader){
    shader->SetVec3("lightPos", m_LightPos);
    shader->SetVec3("lightCol", m_LightCol);
    shader->SetFloat("constant", m_Constant);
    shader->SetFloat("linear", m_Linear);
    shader->SetFloat("quadratic", m_Quadratic);

    for (size_t i = 0; i < m_Models.size(); i++){
        shader->SetMat4("model", m_ModelMats[i]);
        m_Models[i]->Draw(camera, shader);
    }
}
