#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window{
    private:
    GLFWwindow* m_Window;

    public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool ShouldClose() const{
        return glfwWindowShouldClose(m_Window);
    }

    void ClearColor(float r, float g, float b) const{
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(r, g, b, 1.0);
    }

    void ClearDepth() const{
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Poll() const{
        glfwPollEvents();
    }

    void SwapBuffers() const{
        glfwSwapBuffers(m_Window);
    }
};

#endif