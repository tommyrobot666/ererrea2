#ifndef G_GAME_STATE_H // next header, just use #pragma once instead
#define G_GAME_STATE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600


class gameState
{
public:
    GLFWwindow *window;
    float lastMouseX;
    float mouseVelX, mouseVelY;
    glm::vec3 cameraPos; 
    glm::vec3 cameraDir;
    float yaw;
    float pitch;


    gameState();

    void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    void onFrameEnd();
};

#endif