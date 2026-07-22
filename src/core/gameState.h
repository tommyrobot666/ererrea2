#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <core/scene.h>
#include <core/renderer.h>
#include <core/inputManager.h>


class GameState {
    Scene *nextScene = nullptr;
public:
    double gameWindowWidth = 800, gameWindowHeight = 600;
    GLFWwindow *window = nullptr;
    double lastMouseX = gameWindowWidth / 2, lastMouseY = gameWindowHeight / 2;
    double mouseVelX = 0, mouseVelY = 0;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDir = glm::normalize(cameraPos - glm::vec3(0.0f, 0.0f, 1.0f));
    double yaw = 0.0f;
    double pitch = 0.0f;
    double lastFrameTime = glfwGetTime();
    double deltaTime = 0;
    Scene *currentScene = nullptr;
    Renderer *pointerToRenderer = nullptr; // this will be init manualy
    InputManager *input = nullptr;
    glm::mat4 view = glm::mat4(1.0f);
    bool switchingScene = false; // use this to detect if it's the last frame

    GameState() = default;

    GameState(const GameState&)=delete;
    GameState& operator=(const GameState&)=delete;
    GameState(const GameState&&)=delete;
    GameState& operator=(const GameState&&)=delete;

    void initGameState(GLFWwindow *window,Renderer *pointerToRenderer);

    void closingGame();

    static void sizeCallback(GLFWwindow *window, int width, int height);

    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);

    void onFrameStart();

    void onFrameEnd();

    void switchScene(Scene* newNextScene);

    [[nodiscard]] Renderer& r() const {
        return *this->pointerToRenderer;
    }
};

extern GameState gs;
