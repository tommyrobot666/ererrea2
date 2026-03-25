#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gameState.h>
#include <renderer.h>
#include <vertexObjectGenerators.h>
#include <testScene.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

void testScene::load() {
    // get textures and stuff
    gs.cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    texture = renderer::loadPngTexture("smile.png");
    VAO = gs.r().createVertexObject(vertexObjectGenerators::basicCube::vertices(), {},
                                  0, vertexObjectGenerators::basicCube::verticesSize()).VAO;
}

void testScene::simulate() {
    // The code that I wrote when folowing the tutorial
    GLFWwindow *window = gs.window;
    glm::vec3 cameraPos = gs.cameraPos;
    glm::vec3 cameraDir = gs.cameraDir;
    double yaw = gs.yaw;
    double pitch = gs.pitch;
    double mouseVelX = gs.mouseVelX;
    double mouseVelY = gs.mouseVelY;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    constexpr float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        yaw -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        yaw += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pitch -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pitch += 0.1f;

    yaw -= mouseVelX * 0.005;
    pitch -= mouseVelY * 0.005;

    (cameraDir).x = cos(yaw) * cos(pitch);
    (cameraDir).y = sin(pitch);
    (cameraDir).z = sin(yaw) * cos(pitch);
}

void testScene::render() {
    // The code that I wrote when folowing the tutorial
    GLFWwindow *window = gs.window;
    glm::vec3 cameraPos = gs.cameraPos;
    glm::vec3 cameraDir = gs.cameraDir;

    double time = glfwGetTime();

    glClearColor(std::sin(time), std::sin(time + (3.14 / 2)), std::cos(time), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // fov, aspect ratio, near plane distance, far plane distance
    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float) GAME_WINDOW_WIDTH / (float) GAME_WINDOW_HEIGHT, 0.1f,
                                      100.0f);

    glBindTexture(GL_TEXTURE_2D, texture);
    // glBindVertexArray(VAO); TODO

    for (size_t i = 0; i < 10; i++) {
        // object transform
        auto trans = glm::mat4(1.0f); // identity
        trans = glm::translate(trans, cubePositions[i]); // now translation
        trans = glm::rotate(trans, static_cast<float>(time), glm::vec3(0.0, 0.5, 1.0)); // also rotate
        trans = glm::scale(trans, glm::vec3(.5f, .5f, .5f)); // and scale

        // trans is reused as final position
        trans = proj * gs.view * trans;

        gs.r().setShaderTransform(&trans);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    // glBindVertexArray(0); // i think this is the reason why glBindVertexArray(VAO) gets called again TODO
}
