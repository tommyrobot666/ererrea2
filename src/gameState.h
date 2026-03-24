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
    float lastMouseX, lastMouseY;
    float mouseVelX, mouseVelY;
    glm::vec3 cameraPos; 
    glm::vec3 cameraDir;
    float yaw;
    float pitch;
    scene* currentScene;
    scene* nextScene;
    renderer r; // this will be init manualy
    glm::mat4 view;

    gameState();

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    void onFrameStart();

    void onFrameEnd();
};

static gameState gs = gameState();

#endif