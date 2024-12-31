//
//  cameraSystem.cpp
//  LearnOpenGL
//
//  Created by zhangxinjie01 on 2020/12/17.
//  Copyright © 2020 ZHANG. All rights reserved.
//

#include "cameraSystem.hpp"

CameraSystem::CameraSystem(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    m_front(glm::vec3(0.0, 0.0, -1.0)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    m_position = position;
    m_world_up = up;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

CameraSystem::CameraSystem(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    m_position = glm::vec3(posX, posY, posZ);
    m_world_up = glm::vec3(upX, upY, upZ);
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

glm::mat4 CameraSystem::GetViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void CameraSystem::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 temp_front;
    temp_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    temp_front.y = sin(glm::radians(m_pitch));
    temp_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(temp_front);
    
    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void CameraSystem::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        m_position += m_front * velocity;
    if (direction == BACKWARD)
        m_position -= m_front * velocity;
    if (direction == LEFT)
        m_position -= m_right * velocity;
    if (direction == RIGHT)
        m_position += m_right * velocity;
}

void CameraSystem::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void CameraSystem::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}
