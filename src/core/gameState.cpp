#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/gameState.h>

void GameState::initGameState(GLFWwindow *window,Renderer *pointerToRenderer) {
    this->window = window;
    this->pointerToRenderer = pointerToRenderer;
    this->input = new InputManager(window);
}

void GameState::sizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0,0,width,height);
    gs.gameWindowWidth = width;
    gs.gameWindowHeight = height;
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
    input->nextFrame();

    if (switchingScene) {
        currentScene->tryClose();
        delete(currentScene);
        currentScene = nextScene;
        currentScene->tryLoad();
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
        currentScene->tryLoad();
    }
}

GameState gs = GameState();
