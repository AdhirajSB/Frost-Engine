#include "engine.h"

Engine::Engine(int width, int height, const std::string& title){
    m_Width = width;
    m_Height = height;
    m_Window = new Window(width, height, title);
}

Engine::~Engine(){
    delete m_Window;
}

void Engine::Loop(){
    while (!m_Window->ShouldClose()){
        m_Window->ClearColor(1.0, 1.0, 1.0);

        Render();

        m_Window->SwapBuffers();
        m_Window->Poll();
    }
}

void Engine::Render(){
    
}
