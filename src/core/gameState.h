#ifndef G_GAME_STATE_H // next header, just use #pragma once instead
#define G_GAME_STATE_H

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <../scene.h>
#include <../renderer.h>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600


class gameState {
    bool initCalled = false; // for debuging
    scene *nextScene = nullptr;
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
    scene *currentScene = nullptr;
    renderer *pointerToRenderer = nullptr; // this will be init manualy
    glm::mat4 view = glm::mat4(1.0f);
    bool switchingScene = false; // use this to detect if it's the last frame

    gameState() = default;

    gameState(const gameState&)=delete;
    gameState& operator=(const gameState&)=delete;
    gameState(const gameState&&)=delete;
    gameState& operator=(const gameState&&)=delete;

    void initGameState(GLFWwindow *window,renderer *pointerToRenderer);

    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);

    void onFrameStart();

    void onFrameEnd();

    void switchScene(scene* newNextScene);

    [[nodiscard]] renderer& r() const {
        return *this->pointerToRenderer;
    }
};

extern gameState gs;

#endif
