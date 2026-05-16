#include "../otherTestScene.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/gameState.h"
#include "../vertexObjectGenerators.h"

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

const char* defaultVertexShaderSource2 = {
#include <shaders/default.vert>
};
const char* defaultFragmentShaderSource2 = {
#include <shaders/default.frag>
};

glm::vec3 * otherTestScene::cubePoses() {
    return new glm::vec3[] {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
}

void processInput(GLFWwindow *window, glm::vec3 *cameraPos, float *yaw, float *pitch)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);



    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        *cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        *cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        *cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        *cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        *yaw -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        *yaw += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        *pitch -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        *pitch += 0.1f;

    *yaw -= gs.mouseVelX*0.005;
    *pitch -= gs.mouseVelY*0.005;
}

void simulateFrame(GLFWwindow *window, glm::vec3 *cameraPos, float *yaw, float *pitch, glm::vec3 *cameraDir){
    (*cameraDir).x = cos(*yaw) * cos(*pitch);
    (*cameraDir).y = sin(*pitch);
    (*cameraDir).z = sin(*yaw) * cos(*pitch);

    processInput(window,cameraPos,yaw,pitch);
}

void renderFrame(vertexObject& VO, unsigned int texture, glm::vec3 cubePositions[]){
    double time = glfwGetTime();

    renderer::clear(std::sin(time),std::sin(time+(3.14/2)),std::cos(time),1.0);

    // fov, aspect ratio, near plane distance, far plane distance
    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)GAME_WINDOW_WIDTH/(float)GAME_WINDOW_HEIGHT, 0.1f, 100.0f);

    gs.r().defaultShader();
    renderer::currentTexture(texture);
    VO.currentBind();

    for (size_t i = 0; i < 10; i++){
        // object transform
        glm::mat4 trans = glm::mat4(1.0f); // identity
        trans = glm::translate(trans, cubePositions[i]); // now translation
        trans = glm::rotate(trans, static_cast<float>(time), glm::vec3(0.0, 0.5, 1.0)); // also rotate
        trans = glm::scale(trans, glm::vec3(.5f, .5f, .5f)); // and scale

        // trans is reused as final position
        trans = proj * gs.view * trans;

        gs.r().setShaderTransform(&trans);
        VO.drawArray();
    }

    //floor
    auto trans = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f)),
                                         glm::vec3(30.0f, 1.0f, 30.0f));
    trans = proj * gs.view * trans;
    gs.r().setShaderTransform(&trans);
    VO.drawArray();

    glBindVertexArray(0); // i think this is the reason why glBindVertexArray(VAO) gets called again
}



void otherTestScene::load() {
    this->VO = renderer::createVertexObject(vertexObjectGenerators::basicCube::vertices(),{},
        vertexObjectGenerators::basicCube::verticesSize()*vertexObjectGenerators::SizeOfVertex,0);
    this->texture = renderer::loadPngTexture("smile.png");

    gs.cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    gs.cameraDir = glm::normalize(gs.cameraPos - glm::vec3(0.0f, 0.0f, 1.0f));
    gs.yaw = 0.0f;
    gs.pitch = 0.0f;

    glfwSetInputMode(gs.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void otherTestScene::render() {
    renderFrame(*this->VO,this->texture,cubePoses());
}

void otherTestScene::simulate() {
    simulateFrame(gs.window,&gs.cameraPos,reinterpret_cast<float*>(&gs.yaw),reinterpret_cast<float*>(&gs.pitch),&gs.cameraDir);
}


