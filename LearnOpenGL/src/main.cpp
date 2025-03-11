#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include <iostream>
#include <cmath>

#include "shader.hpp"
#include "stb_image.h"
#include "cameraSystem.hpp"

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
    
    // 立方体顶点
    float vertices[] = {
        // Positions          // Normals           // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // Positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    //绑定一个VBO对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //将顶点数据复制到缓冲内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // bind the Vertex Array Object first
    glBindVertexArray(VAO);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 加载纹理
    unsigned int specularMap;
    glGenTextures(1, &specularMap);
    int width, height, nrChannels;
    std::string filePath = PROJECT_PATH + "/resource/container2_border.png";
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int diffuseMap;
    glGenTextures(1, &diffuseMap);
    filePath = PROJECT_PATH + "/resource/container2.png";
    unsigned char *data2 = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //---------> 5. 创建着色器对象
    Shader ourShader(VERRTEX_COLOR_PATH.c_str(), FRAG_COLOR_PATH.c_str());
    Shader lightSharder(LIGHT_VERRTEX_COLOR_PATH.c_str(), LIGHT_FRAG_COLOR_PATH.c_str());
    ourShader.use();
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);
    
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
        glm::vec3 lightColor = glm::vec3(1.0f);
        glm::vec3 objectColor = glm::vec3(1.0f);
        ourShader.setVec3("objectColor", objectColor);

        ourShader.setVec3("viewPos", camera.m_position);

        ourShader.setFloat("material.shininess", 32.0f);
        ourShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f) * lightColor);
        ourShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f) * lightColor);
        ourShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        // ourShader.setVec3("light.position", lightPos);
        // ourShader.setVec3("light.direction", lightDir);
        ourShader.setFloat("light.constant", 1.0f);
        ourShader.setFloat("light.linear", 0.09f);
        ourShader.setFloat("light.quadratic", 0.032f);

        ourShader.setVec3("light.position", camera.m_position);
        ourShader.setVec3("light.direction", camera.m_front);
        ourShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f))); // 注意这里使用了 glm::cos 来获取角度的余弦值，而不是角度本身，节省性能。
        ourShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

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
        model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        ourShader.setMat4("model", model);
        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        glBindVertexArray(VAO);
        for(GLuint i = 0; i < 10; i++)
        {
            model = glm::mat4();
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
        // light cube
        lightSharder.use();
        lightSharder.setMat4("projection", projection);
        lightSharder.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.05f)); // a smaller cube
        lightSharder.setMat4("model", model);
        
        // 绘制灯立方体对象
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
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
