#ifndef ENGINE_H
#define ENGINE_H

#include "window.h"
#include "shader.h"
#include "model.h"
#include "camera.h"

class Engine{
    private:
    Window* m_Window;
    Shader* m_Shader;
    Model* m_Model;
    Camera* m_Camera;
    
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