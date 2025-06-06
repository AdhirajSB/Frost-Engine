#ifndef ENGINE_H
#define ENGINE_H

#include "window.h"
#include "shader.h"
#include "modelManager.h"
#include "camera.h"
#include "gui.h"
#include "gridlines.h"

class Engine{
    private:
    Window* m_Window;
    Shader* m_Shader;
    Shader* m_PickShader;
    ModelManager* m_ModelManager;
    Camera* m_Camera;
    GUI* m_Gui;
    Grid* m_Grid;
    
    int m_Width;
    int m_Height;

    glm::mat4 m_View;
    glm::mat4 m_Projection;

    protected: // TO:DO - Study OOPS
    void Render(); 

    public:
    Engine(const std::string& title);
    ~Engine();

    void Loop();
};

#endif