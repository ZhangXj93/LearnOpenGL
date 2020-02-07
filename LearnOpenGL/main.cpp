#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "shader.hpp"

#define VERRTEX_COLOR_PATH "/Users/zhangxinjie01/OpenGL/LearnOpenGL/LearnOpenGL/resource/vertexcolor.vex"
#define FRAG_COLOR_PATH "/Users/zhangxinjie01/OpenGL/LearnOpenGL/LearnOpenGL/resource/fragcolor.frag"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void draw(GLFWwindow *window);

int main()
{
    glfwInit(); //初始化GLFW
    
    //配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //告知系统我们使用的opengl版本是3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //同样明确告诉GLFW我们使用的是核心模式(Core-profile)
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //MAC环境下需加这一句才能使以上配置生效
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //设置视口
    glViewport(0, 0, 800, 600);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
    };
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    
    // bind the Vertex Array Object first
    glBindVertexArray(VAO);
    //绑定一个VBO对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //将顶点数据复制到缓冲内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //---------> 5. 创建着色器对象
    Shader ourShader(VERRTEX_COLOR_PATH, FRAG_COLOR_PATH);
    
    
    //循环渲染
    while(!glfwWindowShouldClose(window))
    {
        //检测输入事件
        processInput(window);
        
        //渲染指令
        draw(window);
        
        //------> 8. 画三角形
        ourShader.use();
        ourShader.setFloat("ourColor", 1.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
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
}

void draw(GLFWwindow *window)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
