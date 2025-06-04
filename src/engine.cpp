#include "engine.h"

Engine::Engine(const std::string& title){
    m_Window = new Window(1600, 900, title);
    m_Shader = new Shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
    m_PickShader = new Shader("res/shaders/objInfoVS.glsl", "res/shaders/objInfoFS.glsl");
    m_OutlineShader = new Shader("res/shaders/outlineVS.glsl", "res/shaders/outlineFS.glsl");
    m_ModelManager = new ModelManager;
    m_Camera = new Camera(m_Window->GetWindow(), glm::vec3(0.0f, 1.2f, -5.0f));
    m_Gui = new GUI(m_Window->GetWindow());
}

Engine::~Engine(){
    delete m_Window;
    delete m_Shader;
    delete m_PickShader;
    delete m_OutlineShader;
    delete m_ModelManager;
    delete m_Camera;
    delete m_Gui;
}


// Main game loop ->
void Engine::Loop(){
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!m_Window->ShouldClose()){
        m_Window->Clear();

        // Evaluate deltaTime for camera movements
        float currFrame = static_cast<float>(glfwGetTime());
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        // Draw everything
        m_Camera->Update(deltaTime);
        Render();
        int width, height;
        glfwGetWindowSize(m_Window->GetWindow(), &width, &height);
        m_Gui->Render(m_ModelManager, m_View, m_Projection, width, height);

        m_Window->SwapBuffers();
        m_Window->Poll();
    }
}

void Engine::Render(){
    // Setup the width and height according to the display
    glfwGetFramebufferSize(m_Window->GetWindow(), &m_Width, &m_Height); 
    glViewport(0, 0, m_Width, m_Height);

    m_View = m_Camera->ViewMatrix();
    m_Projection = m_Camera->ProjectionMatrix((float)m_Width/m_Height);

    m_PickShader->SetMat4("view", m_View);
    m_PickShader->SetMat4("projection", m_Projection);

    m_ModelManager->Clear();
    m_ModelManager->DrawPicking(m_PickShader);
    
    if (glfwGetMouseButton(m_Window->GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        m_ModelManager->EnablePicking();

        double mouseX, mouseY;
        glfwGetCursorPos(m_Window->GetWindow(), &mouseX, &mouseY);

        int winWidth, winHeight;
        glfwGetWindowSize(m_Window->GetWindow(), &winWidth, &winHeight);

        mouseX *= (double)m_Width/winWidth;
        mouseY *= (double)m_Height/winHeight;

        glReadBuffer(GL_COLOR_ATTACHMENT0);
        unsigned int pixel;
        glReadPixels(mouseX, m_Height - mouseY, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);

        m_ModelManager->selectedModel = (size_t)pixel;
        m_ModelManager->DisablePicking();
    }

    m_Shader->SetMat4("view", m_View);
    m_Shader->SetMat4("projection", m_Projection);
    m_OutlineShader->SetMat4("view", m_View);
    m_OutlineShader->SetMat4("projection", m_Projection);

    m_ModelManager->DrawAll(m_Shader, m_OutlineShader);
}
