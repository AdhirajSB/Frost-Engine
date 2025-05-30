#ifndef ENGINE_H
#define ENGINE_H

#include "window.h"

class Engine{
    private:
    Window* m_Window;
    int m_Width;
    int m_Height;

    protected: // TO:DO - Study OOPS
    void Render(); 

    public:
    Engine(int width, int height, const std::string& title);
    ~Engine();

    void Loop();
};

#endif