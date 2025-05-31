#include "camera.h"

Camera::Camera(GLFWwindow *window, glm::vec3 cameraPos): m_Window(window), m_Position(cameraPos){
    m_Up = m_WorldUp;
    m_Direction = glm::vec3(0.0f, 0.0f, 1.0f);

    glfwSetWindowUserPointer(m_Window, this);
    glfwSetCursorPosCallback(m_Window, MouseMovement);
}

void Camera::Update(float deltaTime){
    const float velocity = m_Speed * deltaTime;

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS){
        m_Position += glm::normalize(m_Direction) * velocity;
    }
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS){
        m_Position -= glm::normalize(m_Direction) * velocity;
    }
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS){
        m_Position += glm::normalize(m_Right) * velocity;
    }
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS){
        m_Position -= glm::normalize(m_Right) * velocity;
    }
}

void MouseMovement(GLFWwindow *window, double xPos, double yPos){
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam) return;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) != GLFW_PRESS){
        cam->m_FirstMouse = true;
        return;
    }

    if (cam->m_FirstMouse){
        cam->m_LastX = (float)xPos;
        cam->m_LastY = (float)yPos;
        cam->m_FirstMouse = false;
    }

    float xOffset = (float)xPos - cam->m_LastX;
    float yOffset = cam->m_LastY - (float)yPos;

    cam->m_LastX = (float)xPos;
    cam->m_LastY = (float)yPos;


    xOffset *= cam->m_Sensitivity;
    yOffset *= cam->m_Sensitivity;

    cam->m_Yaw += xOffset;
    cam->m_Pitch += yOffset;

    if (cam->m_Pitch > 89.0f) cam->m_Pitch = 89.0f;
    else if (cam->m_Pitch < -89.0f) cam->m_Pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(cam->m_Yaw)) * cos(glm::radians(cam->m_Pitch));
    direction.y = sin(glm::radians(cam->m_Pitch));
    direction.z = sin(glm::radians(cam->m_Yaw)) * cos(glm::radians(cam->m_Pitch));

    cam->m_Direction = glm::normalize(direction);
    cam->m_Right = glm::normalize(glm::cross(cam->m_Direction, cam->m_WorldUp));
    cam->m_Up = glm::normalize(glm::normalize(glm::cross(cam->m_Right, cam->m_Direction)));
}
