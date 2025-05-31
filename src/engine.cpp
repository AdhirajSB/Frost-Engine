#include "engine.h"
#include "vendor/tinyfiledialogs.h"

Engine::Engine(const std::string& title){
    m_Window = new Window(1600, 900, title);
    m_Shader = new Shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
    m_Model = new Model("res/sampleModels/LaraCroft/Head.obj");
    m_Camera = new Camera(m_Window->GetWindow(), glm::vec3(0.0f));
}

Engine::~Engine(){
    delete m_Window;
    delete m_Shader;
    delete m_Model;
    delete m_Camera;
}


// Main game loop ->
void Engine::Loop(){
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!m_Window->ShouldClose()){
        m_Window->ClearColor(0.0, 0.0, 0.0);

        // Evaluate deltaTime for camera movements
        float currFrame = static_cast<float>(glfwGetTime());
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        // Draw everything
        m_Camera->Update(deltaTime);
        Render();

        m_Window->SwapBuffers();
        m_Window->Poll();
    }
}

void Engine::Render(){
    // Setup the width and height according to the display
    glfwGetFramebufferSize(m_Window->GetWindow(), &m_Width, &m_Height); 
    glViewport(0, 0, m_Width, m_Height);


    m_Shader->Use();
    m_Shader->SetMat4("model", glm::mat4(1.0f));
    m_Shader->SetMat4("view", m_Camera->ViewMatrix());
    m_Shader->SetMat4("projection", m_Camera->ProjectionMatrix((float)m_Width/m_Height));
    m_Model->Draw();
}
