//
//  cameraSystem.hpp
//  LearnOpenGL
//
//  Created by zhangxinjie01 on 2020/12/17.
//  Copyright © 2020 ZHANG. All rights reserved.
//

#ifndef cameraSystem_hpp
#define cameraSystem_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;

const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


class CameraSystem
{
public:
    CameraSystem(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                 float yaw = YAW, float pitch = PITCH);
    
    // constructor with scalar values
    CameraSystem(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    
    // 获取观察矩阵
    glm::mat4 GetViewMatrix();
    
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);
    
    // camera参数
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_world_up;
    glm::vec3 m_right;
    
    float m_yaw;
    float m_pitch;
    
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};

#endif /* cameraSystem_hpp */
