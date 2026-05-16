#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <../gameState.h>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

void gameState::initGameState(GLFWwindow *window,renderer *pointerToRenderer) {
    this->window = window;
    this->pointerToRenderer = pointerToRenderer;
    initCalled = true;
}

void gameState::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    gs.mouseVelX = gs.lastMouseX - xpos;
    gs.mouseVelY = gs.lastMouseY - ypos;

    gs.lastMouseX = xpos;
    gs.lastMouseY = ypos;
}

void gameState::onFrameStart() {
    deltaTime = glfwGetTime() - lastFrameTime;

    // camera location
    view = glm::lookAt(cameraPos, cameraPos + cameraDir, glm::vec3(0.0, 1.0, 0.0));

    if (nextScene != nullptr) {
        switchingScene = true;
    }
}

void gameState::onFrameEnd() {
    mouseVelX = 0;
    mouseVelY = 0;
    lastFrameTime = glfwGetTime();

    if (switchingScene) {
        delete(currentScene);
        currentScene = nextScene;
        currentScene->load();
        nextScene = nullptr;
        switchingScene = false;
    }
}

void gameState::switchScene(scene* newNextScene) {
    if (currentScene) {
        assert(nextScene == nullptr);
        nextScene = newNextScene;
    } else {
        currentScene = newNextScene;
        currentScene->load();
    }
}

gameState gs = gameState();
