#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gameState.h>

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


    gameState();

    void mouseCallback(GLFWwindow* window, double xpos, double ypos){
        mouseVelX = lastMouseX - xpos;
        mouseVelY = lastMouseY - ypos;

        lastMouseX = xpos;
        lastMouseY = ypos;
    }

    void onFrameEnd(){
        mouseVelX = 0;
        mouseVelY = 0;
    }
};