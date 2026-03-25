#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gameState.h>
#include <renderer.h>
#include <testScene.h>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, "tErerrea2", NULL, NULL);
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
    glViewport(0, 0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);
    renderer coreRenderer = renderer();
    // gs.pointerToRenderer = &coreRenderer;
    gs.initGameState(window,&coreRenderer);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, gameState::mouseCallback);

    testScene test = testScene(); // the new keyword does exist and makes it on the heap
    gs.currentScene = &test;
    test.load();

    while (!glfwWindowShouldClose(window)) {
        gs.onFrameStart();
        simulateFrame();
        renderFrame();
        gs.onFrameEnd();


        glfwSwapBuffers(window); //it draws to back buffer first, this swaps it to be the shown front buffer
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
