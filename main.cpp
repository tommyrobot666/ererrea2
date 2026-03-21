#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

void simulateFrame(GLFWwindow *window);
void renderFrame(GLFWwindow *window);

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

    while(!glfwWindowShouldClose(window))
    {
        simulateFrame(window);
        renderFrame(window);

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

void renderFrame(GLFWwindow *window){
    std::chrono::time_point time_point = std::chrono::system_clock::now();
    std::chrono::duration time_duration = time_point - time_point.min();
    float time = time_duration.count();

    glClearColor(std::sinf(time),std::sinf(time+(3.14/2)),std::cosf(time),1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    std::cout << std::sinf(time);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}