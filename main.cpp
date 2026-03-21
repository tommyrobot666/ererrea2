#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

const char* defaultVertexShaderSource = {
    #include <shaders/default.vert>
};
const char* defaultFragmentShaderSource = {
    #include <shaders/default.frag>
};

void simulateFrame(GLFWwindow *window);
void renderFrame(GLFWwindow *window, unsigned int VAO, unsigned int shaderProgram);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, "tErerrea2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // this starts glad, but it's in a messy if because it also returns if it worked
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ //functions can be casted?
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };  
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //GL_STATIC_DRAW is for unchanging things, GL_DYNAMIC_DRAW is for changing things
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    int  success;
    char infoLog[512];
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &defaultVertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Default vertex shader failed\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &defaultFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Default fragment shader failed\n" << infoLog << std::endl;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program failed\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); // the shaders are in the program now
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // tell it how vertices is formatted
    glEnableVertexAttribArray(0);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);




    while(!glfwWindowShouldClose(window))
    {
        simulateFrame(window);
        renderFrame(window,VAO,shaderProgram);

        glfwSwapBuffers(window); //it draws to back buffer first, this swaps it to be the shown front buffer
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window);
void simulateFrame(GLFWwindow *window){
    processInput(window);
}

void renderFrame(GLFWwindow *window, unsigned int VAO, unsigned int shaderProgram){
    std::chrono::time_point time_point = std::chrono::system_clock::now();
    std::chrono::duration time_duration = time_point.time_since_epoch();
    double time = time_duration.count(); //for some reason, using float makes time not change
    time *= .0000001; // colors were too fast

    glClearColor(std::sin(time),std::sin(time+(3.14/2)),std::cos(time),1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    std::cout << std::sin(time) << "   " << time << "\n";

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}