#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/gameState.h>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

void GameState::initGameState(GLFWwindow *window,Renderer *pointerToRenderer) {
    this->window = window;
    this->pointerToRenderer = pointerToRenderer;
    initCalled = true;
}

void GameState::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    gs.mouseVelX = gs.lastMouseX - xpos;
    gs.mouseVelY = gs.lastMouseY - ypos;

    gs.lastMouseX = xpos;
    gs.lastMouseY = ypos;
}

void GameState::onFrameStart() {
    deltaTime = glfwGetTime() - lastFrameTime;

    // camera location
    cameraDir.x = cos(yaw) * cos(pitch);
    cameraDir.y = sin(pitch);
    cameraDir.z = sin(yaw) * cos(pitch);
    view = glm::lookAt(cameraPos, cameraPos + cameraDir, glm::vec3(0.0, 1.0, 0.0));

    if (nextScene != nullptr) {
        switchingScene = true;
    }
}

void GameState::onFrameEnd() {
    mouseVelX = 0;
    mouseVelY = 0;
    lastFrameTime = glfwGetTime();

    if (switchingScene) {
        currentScene->close();
        delete(currentScene);
        currentScene = nextScene;
        currentScene->load();
        nextScene = nullptr;
        switchingScene = false;
    }
}

void GameState::switchScene(Scene* newNextScene) {
    if (currentScene) {
        assert(nextScene == nullptr);
        nextScene = newNextScene;
    } else {
        // when the games just started, no scene
        currentScene = newNextScene;
        currentScene->load();
    }
}

GameState gs = GameState();
