#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
    
    //循环渲染
    while(!glfwWindowShouldClose(window))
    {
        //检测输入事件
        processInput(window);
        
        //渲染指令
        draw(window);
        
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
