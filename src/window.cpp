#include "window.h"

Window::Window(int width, int height, const std::string &title): m_Window(nullptr){
    if (!glfwInit()){
        std::cerr << "Failed to initialize glfw" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (m_Window == nullptr){
        std::cerr << "Failed to create window" << std::endl;
        return;
    }
    glfwMakeContextCurrent(m_Window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK){
        std::cerr << "Failed to initialize glew" << std::endl;
        return;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
}

Window::~Window(){
    if (m_Window) glfwDestroyWindow(m_Window);
    glfwTerminate();
}
