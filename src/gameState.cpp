#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <gameState.h>
#include <scene.h>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

class gameState
{
public:
    GLFWwindow *window;
    float lastMouseX = GAME_WINDOW_WIDTH/2, lastMouseY = GAME_WINDOW_HEIGHT/2;
    float mouseVelX = 0, mouseVelY = 0;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f); 
    glm::vec3 cameraDir = glm::normalize(cameraPos - glm::vec3(0.0f, 0.0f, 1.0f));
    float yaw = 0.0f;
    float pitch = 0.0f;
    float lastFrameTime = glfwGetTime();
    float deltaTime = 0;
    scene* currentScene;
    scene* nextScene;

    gameState();

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos){
        gs.mouseVelX = gs.lastMouseX - xpos;
        gs.mouseVelY = gs.lastMouseY - ypos;

        gs.lastMouseX = xpos;
        gs.lastMouseY = ypos;
    }

    void onFrameStart(){
        deltaTime = glfwGetTime() - lastFrameTime;
    }

    void onFrameEnd(){
        mouseVelX = 0;
        mouseVelY = 0;
        lastFrameTime = glfwGetTime();
    }
};