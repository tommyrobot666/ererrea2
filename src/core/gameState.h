#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <core/scene.h>
#include <core/inputManager.h>


class GameState {
    Scene *nextScene = nullptr;
public:
    double gameWindowWidth = 800, gameWindowHeight = 600; // set in callback
    GLFWwindow *window = nullptr;
    double lastMouseX = gameWindowWidth / 2, lastMouseY = gameWindowHeight / 2; // set in callback
    double mouseVelX = 0, mouseVelY = 0; //set in endFrame
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDir = glm::normalize(cameraPos - glm::vec3(0.0f, 0.0f, 1.0f)); // set in startFrame
    double yaw = 0.0f;
    double pitch = 0.0f;
    double lastFrameTime = glfwGetTime(); // set in startFrame
    double deltaTime = 0; //set in endFrame
    Scene *currentScene = nullptr;
    InputManager *input = nullptr;
    glm::mat4 view = glm::mat4(1.0f); // set in startFrame
    bool switchingScene = false; // use this to detect if it's the last frame // set in startFrame

    GameState() = default;

    GameState(const GameState&)=delete;
    GameState& operator=(const GameState&)=delete;
    GameState(const GameState&&)=delete;
    GameState& operator=(const GameState&&)=delete;

    void initGameState(GLFWwindow *window);
    void closingGame();
    // set gameWindow and glViewport
    static void sizeCallback(GLFWwindow *window, int width, int height);
    // set lastMouse and delta
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    // set camDir; reset lastTime; detect scene switch
    void onFrameStart();
    // reset mouseVel; step inputSys; set deltaTime; Do the scene switching
    void onFrameEnd();
    void switchScene(Scene* newNextScene);
};

extern GameState gs;
