#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

void MouseMovement(GLFWwindow* window, double xPos, double yPos);

class Camera{
    private:
    GLFWwindow* m_Window;
    glm::vec3 m_Position, m_Direction, m_Up, m_Right;
    const glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_Fov = 45.0f;
    float m_Yaw = -90.0f, m_Pitch = 0.0f;
    float m_Speed = 2.5f;

    bool m_FirstMouse = true;
    float m_Sensitivity = 0.1f;
    float m_LastX = 1600.0f / 2.0f;
    float m_LastY = 900.0f / 2.0f;

    friend void MouseMovement(GLFWwindow* window, double xPos, double Ypos);

    public:
    Camera(GLFWwindow* window, glm::vec3 cameraPos);
    
    inline glm::mat4 ViewMatrix() const{
        return glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
    }
    inline glm::mat4 ProjectionMatrix(float aspectRatio) const{
        return glm::perspective(glm::radians(m_Fov), aspectRatio, 0.1f, 100.0f);
    }
    inline glm::vec3 CameraPos() const{
        return m_Position;
    }

    void Update(float deltaTime);
};

#endif