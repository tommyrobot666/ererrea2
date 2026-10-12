#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/renderer.h>
#include <core/gameState.h>
#include <core/d2renderer.h>

void GameState::initGameState(GLFWwindow *window) {
    assert(window != nullptr);

    this->mainWindow = window;
    renderer.init();
    this->input = new InputManager(window);
}

void GameState::closingGame() const {
    if (currentScene) {
        currentScene->tryClose();
        delete(currentScene);
    }
    if (nextScene) {
        nextScene->tryClose();
        delete(nextScene);
    }
    delete(input);
}

void GameState::sizeCallback(GLFWwindow *window, int width, int height) {
    assert(window == gs.mainWindow);

    glViewport(0,0,width,height);
    gs.gameWindowWidth = width;
    gs.gameWindowHeight = height;
}

void GameState::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    assert(window == gs.mainWindow);

    gs.mouseVelX = gs.lastMouseX - xpos;
    gs.mouseVelY = gs.lastMouseY - ypos;

    gs.lastMouseX = xpos;
    gs.lastMouseY = ypos;
}

void GameState::onFrameStart() {
    lastFrameTime = glfwGetTime();

    // camera location
    cameraDir.x = static_cast<float>(cos(yaw) * cos(pitch));
    cameraDir.y = static_cast<float>(sin(pitch));
    cameraDir.z = static_cast<float>(sin(yaw) * cos(pitch));
    view = glm::lookAt(cameraPos, cameraPos + cameraDir, glm::vec3(0.0, 1.0, 0.0));

    OrthProjManager::updateOrthProjs(static_cast<float>(gameWindowWidth), static_cast<float>(gameWindowHeight));

    if (nextScene != nullptr) {
        switchingScene = true;
    }
}

void GameState::onFrameEnd() {
    mouseVelX = 0;
    mouseVelY = 0;
    deltaTime = glfwGetTime() - lastFrameTime;
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
