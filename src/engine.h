#ifndef ENGINE_H
#define ENGINE_H

#include "window.h"
#include "shader.h"
#include "modelManager.h"
#include "camera.h"
#include "gui.h"

class Engine{
    private:
    Window* m_Window;
    Shader* m_Shader;
    ModelManager* m_ModelManager;
    Camera* m_Camera;
    GUI* m_Gui;
    
    int m_Width;
    int m_Height;

    protected: // TO:DO - Study OOPS
    void Render(); 

    public:
    Engine(const std::string& title);
    ~Engine();

    void Loop();
};

#endif