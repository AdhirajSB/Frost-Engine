#include "modelManager.h"
#include "gui.h"


GUI::GUI(GLFWwindow *window): m_Window(window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

GUI::~GUI(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::Render(ModelManager* modelManager){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Main ->
    if (m_ShowDemoWindow){
        ImGui::ShowDemoWindow(&m_ShowDemoWindow);
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")){
            if (ImGui::MenuItem("Import Model")){
                const char* filters[] = {"*.obj"};
                const char* filename = tinyfd_openFileDialog(
                    "Choose a wavefront file",
                    "",
                    1,
                    filters,
                    "Wavefront OBJ",
                    0
                );
                if (filename) modelManager->AddModel(filename);
            }
            if (ImGui::MenuItem("Quit")){
                glfwSetWindowShouldClose(m_Window, true);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}