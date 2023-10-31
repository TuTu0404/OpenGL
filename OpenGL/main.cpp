#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int creatVertexShader(unsigned int* vertexShader, int* success, char* infoLog);
unsigned int creatFragmentShader(unsigned int* fragmentShader, int* success, char* infoLog);
unsigned int linkProgram(unsigned int& vertexShader, unsigned int& fragmentShader, int* success, char* infoLog);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = R"(
    #version 330
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 ourColor;
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 ourColor; 

    void main()
    {
        FragColor = vec4(ourColor,1.0);
    }
)";


int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int success;
    char infoLog[512];
    //创建着色器对象(顶点着色器)
    unsigned int vertexShader;
    vertexShader = creatVertexShader(&vertexShader,&success,infoLog);

    //片段着色器
    unsigned int fragmentShader;
    fragmentShader = creatFragmentShader(&fragmentShader,&success, infoLog);

    //着色器程序,把着色器附加到程序上，然后链接他们
    unsigned int shaderProgram;
    shaderProgram = linkProgram(vertexShader,fragmentShader, &success, infoLog);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    //索引，绘制三角形的顺序
    unsigned int indices[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    //VBO,存储在现存上的顶点属性缓冲，VAO定义了怎么分辨这些顶点属性是颜色还是位置等
    unsigned int VAO,VBO,EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //顶点缓冲
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //索引缓冲
    /*glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //激活着色器
        glUseProgram(shaderProgram);


        //绘制三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        //交换缓冲且查询io事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    //获取输入
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

unsigned int creatVertexShader(unsigned int* vertexShader,int* success, char* infoLog) {
    *vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(*vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(*vertexShader);
    //检测着色器对象是否编译成功

    glGetShaderiv(*vertexShader, GL_COMPILE_STATUS, success);
    if (!*success) {
        glGetShaderInfoLog(*vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return *vertexShader;
}

unsigned int creatFragmentShader(unsigned int* fragmentShader,int* success, char* infoLog) {
    //片段着色器
    *fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(*fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(*fragmentShader);
    // check for shader compile errors
    glGetShaderiv(*fragmentShader, GL_COMPILE_STATUS, success);
    if (!*success)
    {
        glGetShaderInfoLog(*fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return *fragmentShader;
}

unsigned int linkProgram(unsigned int &vertexShader, unsigned int &fragmentShader, int* success, char* infoLog) {
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, success);
    if (!*success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return shaderProgram;
}

