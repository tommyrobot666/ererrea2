#ifndef G_GAME_STATE_H // next header, just use #pragma once instead
#define G_GAME_STATE_H

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <core/scene.h>
#include <core/renderer.h>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600


class GameState {
    bool initCalled = false; // for debuging
    Scene *nextScene = nullptr;
public:
    GLFWwindow *window = nullptr;
    double lastMouseX = GAME_WINDOW_WIDTH / 2, lastMouseY = GAME_WINDOW_HEIGHT / 2;
    double mouseVelX = 0, mouseVelY = 0;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDir = glm::normalize(cameraPos - glm::vec3(0.0f, 0.0f, 1.0f));
    double yaw = 0.0f;
    double pitch = 0.0f;
    double lastFrameTime = glfwGetTime();
    double deltaTime = 0;
    Scene *currentScene = nullptr;
    Renderer *pointerToRenderer = nullptr; // this will be init manualy
    glm::mat4 view = glm::mat4(1.0f);
    bool switchingScene = false; // use this to detect if it's the last frame

    GameState() = default;

    GameState(const GameState&)=delete;
    GameState& operator=(const GameState&)=delete;
    GameState(const GameState&&)=delete;
    GameState& operator=(const GameState&&)=delete;

    void initGameState(GLFWwindow *window,Renderer *pointerToRenderer);

    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);

    void onFrameStart();

    void onFrameEnd();

    void switchScene(Scene* newNextScene);

    [[nodiscard]] Renderer& r() const {
        return *this->pointerToRenderer;
    }
};

extern GameState gs;

#endif
