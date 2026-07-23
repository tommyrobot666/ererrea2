#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/gameState.h>
#include <core/renderer.h>
#include <core/glDebugLogger.h>
#include <uicore/uiObject.h>
#include <test/otherTestScene.h>
#include <ereacore/ereaGameScene.h>

#ifndef RESOURCES_PATH
#define RESOURCES_PATH
#endif

void simulateFrame() {
    if (gs.currentScene) {
        gs.currentScene->simulate();
    }
}

void renderFrame() {
    if (gs.currentScene) {
        gs.currentScene->render();
    }
}

int main() {
    int gameWindowWidth = 800, gameWindowHeight = 600;

    glfwInit();
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // enable debug messsages
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // set GLFW to version 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // if the version is any lower, the debug causes segfault

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(gameWindowWidth, gameWindowHeight, "tErerrea2", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // this starts glad, but it's in a messy if because it also returns if it worked
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        //functions can be casted?
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    glViewport(0, 0,gameWindowWidth,gameWindowHeight);

    // copied and paseted
    // from https://learnopengl.com/In-Practice/Debugging
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        // initialize debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    auto* coreRenderer = new Renderer();
    gs.initGameState(window,coreRenderer);
    uiObject::initUiSystem();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwSetWindowSizeCallback(window, GameState::sizeCallback);
    glfwSetCursorPosCallback(window, GameState::mouseCallback);
    glfwSetKeyCallback(window, InputManager::keyCallback);

    auto *test = new ereaGameScene(); // the new keyword does exist and makes it on the heap
    gs.switchScene(test);

    while (!glfwWindowShouldClose(window)) {
        gs.onFrameStart();
        simulateFrame();
        renderFrame();
        gs.onFrameEnd();


        glfwSwapBuffers(window); //it draws to back buffer first, this swaps it to be the shown front buffer
        glfwPollEvents();
    }

    uiObject::closeUiSystem();
    gs.closingGame();
    glfwTerminate();
    return 0;
}
