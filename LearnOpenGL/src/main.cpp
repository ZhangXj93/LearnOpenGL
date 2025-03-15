#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include <iostream>
#include <cmath>

#include "shader.hpp"
#include "stb_image.h"
#include "cameraSystem.hpp"
#include "model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

const float screen_width = 800.0f;
const float screen_height = 600.0f;
const std::string VERRTEX_COLOR_PATH = (PROJECT_PATH + "/resource/vertexcolor.vex");
const std::string FRAG_COLOR_PATH = (PROJECT_PATH + "/resource/fragcolor.frag");
const std::string LIGHT_VERRTEX_COLOR_PATH = (PROJECT_PATH + "/resource/lightVertexColor.vex");
const std::string LIGHT_FRAG_COLOR_PATH = (PROJECT_PATH + "/resource/lightFragColor.frag");

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
CameraSystem camera(cameraPos);
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

float lastX = screen_width / 2, lastY = screen_height / 2; //记录上一帧的鼠标位置，初始位置屏幕中心
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void draw(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos); //鼠标移动事件监听
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); //鼠标滚轮事件监听

glm::vec3 lightPos(0.6f, 0.5f, 1.0f);
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);

int main()
{
    glfwInit(); //初始化GLFW
    
    //配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //告知系统我们使用的opengl版本是3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //同样明确告诉GLFW我们使用的是核心模式(Core-profile)
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //MAC环境下需加这一句才能使以上配置生效
    
    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetCursorPosCallback(window, mouse_callback); // 添加鼠标事件监听
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 鼠标事件设置，隐藏光标，并捕捉它
    glfwSetScrollCallback(window, scroll_callback); //鼠标滚轮事件
    
    //初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    
    Model ourModel(PROJECT_PATH + "/resource/models/nanosuit/nanosuit.obj");
    
    //---------> 5. 创建着色器对象
    Shader ourShader(VERRTEX_COLOR_PATH.c_str(), FRAG_COLOR_PATH.c_str());
    
    //循环渲染
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //检测输入事件
        processInput(window);
        
        //渲染指令
        draw(window);
        
        ourShader.use();

        //----------> 三大矩阵
        // 观察矩阵
        glm::mat4 view;
        view = camera.GetViewMatrix();
        
        // 投影矩阵
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), screen_width/screen_height, 0.1f, 100.0f);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        
        // 模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        // model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    //正确释放/删除之前的分配的所有资源
    glfwTerminate();
    
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void draw(GLFWwindow *window)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now! 与开启深度测试一起使用
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
