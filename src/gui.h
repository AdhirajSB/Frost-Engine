#ifndef GUI_H
#define GUI_H

#include <GLFW/glfw3.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/ImGuizmo.h"
#include "vendor/tinyfiledialogs.h"

class GUI{
    private:
    GLFWwindow* m_Window;
    bool m_ShowDemoWindow = false;

    ImGuizmo::OPERATION m_CurrentGizmoOperation;

    public:
    GUI(GLFWwindow* window);
    ~GUI();

    void Render(ModelManager* modelManager, const glm::mat4& view, const glm::mat4& projection, int width, int height);
};

#endif