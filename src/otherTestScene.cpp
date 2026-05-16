#include "otherTestScene.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gameState.h"

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

void renderFrame(GLFWwindow *window, unsigned int VAO, unsigned int shaderProgram, unsigned int texture, unsigned int transformLoc, glm::vec3 cubePositions[], glm::vec3 *cameraPos, glm::vec3 *cameraDir){
    double time = glfwGetTime();

    glClearColor(std::sin(time),std::sin(time+(3.14/2)),std::cos(time),1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // fov, aspect ratio, near plane distance, far plane distance
    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)GAME_WINDOW_WIDTH/(float)GAME_WINDOW_HEIGHT, 0.1f, 100.0f);

    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);

    for (size_t i = 0; i < 10; i++){
        // object transform
        glm::mat4 trans = glm::mat4(1.0f); // identity
        trans = glm::translate(trans, cubePositions[i]); // now translation
        trans = glm::rotate(trans, (float)time, glm::vec3(0.0, 0.5, 1.0)); // also rotate
        trans = glm::scale(trans, glm::vec3(.5f, .5f, .5f)); // and scale

        // trans is reused as final position
        trans = proj * gs.view * trans;

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    }
    glBindVertexArray(0); // i think this is the reason why glBindVertexArray(VAO) gets called again
}



void otherTestScene::load() {
    unsigned int shaderProgram = renderer::setUpShaders();
    this->shaderProgram = shaderProgram;
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    this->transformLoc = transformLoc;

    float vertices[288] = {
        // positions           // colors       // uvs
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, 0.5f,    1.0f,1.0f,1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f
    };
    unsigned int indices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    this->VAO = renderer::createVertexObject(vertices,indices,sizeof(vertices),sizeof(indices))->VAO;

    //GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER (like MonoGame)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  // for GL_CLAMP_TO_BORDER

    //mipmap stuff
    // NEAREST is exact value, LINEAR blurrs between
    // GL_[MIPMAP]_MIPMAP_[TEXEL]
    // [MIPMAP] affects interpoliate for mipmap selection
    // [TEXEL] affects interpoliate for pixels
    // 2*2 = 4 mipmap constants for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_x_FILTER, x)

    // GL_NEAREST is exact value, GL_LINEAR blurrs between
    // GL_TEXTURE_MIN_FILTER is scaled down, GL_TEXTURE_MAG_FILTER is scaled up
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    this->texture = renderer::loadPngTexture("smile.png");

    glEnable(GL_DEPTH_TEST);


    gs.cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    gs.cameraDir = glm::normalize(gs.cameraPos - glm::vec3(0.0f, 0.0f, 1.0f));
    gs.yaw = 0.0f;
    gs.pitch = 0.0f;

    glfwSetInputMode(gs.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void otherTestScene::render() {
    renderFrame(gs.window,this->VAO,this->shaderProgram,this->texture,this->transformLoc,cubePoses(),&gs.cameraPos,&gs.cameraDir);
}

void otherTestScene::simulate() {
    simulateFrame(gs.window,&gs.cameraPos,reinterpret_cast<float*>(&gs.yaw),reinterpret_cast<float*>(&gs.pitch),&gs.cameraDir);
}


