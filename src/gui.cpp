#include "modelManager.h"
#include "gui.h"

GUI::GUI(GLFWwindow *window): m_Window(window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable docking
    io.Fonts->AddFontFromFileTTF("res/fonts/DroidSans.ttf", 16.0f);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
}

GUI::~GUI(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::Render(ModelManager* modelManager, const glm::mat4& view, const glm::mat4& projection, int width, int height){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ImGUI dockspace
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("DockSpaceHostWindow", nullptr, host_window_flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();


    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard) { // Only process when ImGui doesn't need keyboard input
        if (ImGui::IsKeyPressed(ImGuiKey_G)) {
            m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_R)) {
            m_CurrentGizmoOperation = ImGuizmo::ROTATE;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_E)) {
            m_CurrentGizmoOperation = ImGuizmo::SCALE;
        }
    }
    
    // Main Menu Bar
    if (m_ShowDemoWindow){
        ImGui::ShowDemoWindow(&m_ShowDemoWindow);
    }
    
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("SceneContextMenu");
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
    }
    if (ImGui::BeginPopup("SceneContextMenu")) {
        if (ImGui::MenuItem("Add Cube")){
            modelManager->AddModel("res/primitives/cube.obj");
        }
        if (ImGui::MenuItem("Add Sphere")){
            modelManager->AddModel("res/primitives/sphere.obj");
        }
        if (ImGui::MenuItem("Add Cylinder")){
            modelManager->AddModel("res/primitives/cylinder.obj");
        }
        if (ImGui::MenuItem("Add Cone")){
            modelManager->AddModel("res/primitives/cone.obj");
        }
        if (ImGui::MenuItem("Add Torus")){
            modelManager->AddModel("res/primitives/torus.obj");
        }
        if (ImGui::MenuItem("Import Model")) {
            const char* filters[] = { "*.obj" };
            const char* filename = tinyfd_openFileDialog(
                "Choose a Wavefront OBJ file",
                "",
                1,
                filters,
                "Wavefront OBJ",
                0
            );
            if (filename) modelManager->AddModel(filename);
        }
        ImGui::EndPopup();
    }

    static glm::vec3 lightPos = glm::vec3(0.0f);
    static glm::vec3 lightCol = glm::vec3(1.0f);
    static float a = 1.0f;
    static float b = 0.14f;
    static float c = 0.07f;
    ImGui::Begin("Light Controls");
    ImGui::SliderFloat3("Position:", glm::value_ptr(lightPos), -10.0f, 10.0f);
    ImGui::Separator();
    ImGui::ColorPicker3("Color", glm::value_ptr(lightCol), ImGuiColorEditFlags_PickerHueWheel);
    ImGui::Separator();
    ImGui::SliderFloat("Base Intensity", &a, 0.0f, 2.0f);
    ImGui::SliderFloat("Falloff Start", &b, 0.0f, 1.0f);
    ImGui::SliderFloat("Falloff Sharpness", &c, 0.0f, 1.0f);

    ImGui::End();
    modelManager->SetupLights(lightPos, lightCol, a, b, c);

    ImGui::Begin("Gizmo Controls");
    ImGui::Text("Keyboard Shortcuts:");
    ImGui::Text("G - Translate");
    ImGui::Text("R - Rotate");
    ImGui::Text("E - Scale");
    ImGui::Separator();
    
    if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE)) {
        m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE)) {
        m_CurrentGizmoOperation = ImGuizmo::ROTATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE)) {
        m_CurrentGizmoOperation = ImGuizmo::SCALE;
    }
    ImGui::End();
    
    if (modelManager->selectedModel != 0){
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
        ImGuizmo::SetRect(0, 0, width, height);
        
        glm::mat4& model = modelManager->GetSelectedModel();
        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            m_CurrentGizmoOperation,
            ImGuizmo::WORLD,
            glm::value_ptr(model),
            nullptr,
            nullptr
        );
    }
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}