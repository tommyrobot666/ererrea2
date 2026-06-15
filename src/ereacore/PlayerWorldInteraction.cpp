#include <ereacore/PlayerWorldInteraction.h>

#include <GLFW/glfw3.h>
#include <3dListUtil.h>
#include <core/gameState.h>
#include <ereacore/chunk.h>

void PlayerWorldInteraction::moveCamera() {
    float sprint = 1.0f;
    if (glfwGetKey(gs.window, GLFW_KEY_LEFT_SHIFT))
        sprint = 68.0f;

    if (glfwGetKey(gs.window, GLFW_KEY_LEFT) == GLFW_PRESS)
        gs.yaw -= 1.3f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        gs.yaw += 1.3f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_DOWN) == GLFW_PRESS)
        gs.pitch -= 1.3f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_UP) == GLFW_PRESS)
        gs.pitch += 1.3f*gs.deltaTime*sprint;

    if (glfwGetKey(gs.window, GLFW_KEY_W) == GLFW_PRESS)
        gs.cameraPos.z += 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_S) == GLFW_PRESS)
        gs.cameraPos.z -= 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_A) == GLFW_PRESS)
        gs.cameraPos.x -= 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_D) == GLFW_PRESS)
        gs.cameraPos.x += 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_Q) == GLFW_PRESS)
        gs.cameraPos.y -= 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_E) == GLFW_PRESS)
        gs.cameraPos.y += 2.7f*gs.deltaTime*sprint;

    if(glfwGetKey(gs.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(gs.window, true);
}

void PlayerWorldInteraction::interactWithUnits(std::vector<Chunk>& chunks) {
    auto lookAtUnitPos = rayCast(chunks,gs.cameraDir,gs.cameraPos,2);

    if (glfwGetKey(gs.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Chunk::setUnitAtGlobalPos(Unit::NONE,chunks,lookAtUnitPos.x,lookAtUnitPos.y,lookAtUnitPos.z);
    } else {
        if (Chunk::getUnitAtGlobalPos(chunks,lookAtUnitPos.x,lookAtUnitPos.y,lookAtUnitPos.z) != Unit::ORE)
        Chunk::setUnitAtGlobalPos(Unit::STONE,chunks,lookAtUnitPos.x,lookAtUnitPos.y,lookAtUnitPos.z);
    }

    if (glfwGetKey(gs.window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        auto lookAtUnitFrontPos = ListUtilVecInt{
                lookAtUnitPos.x-static_cast<int>(gs.cameraDir.x),
                lookAtUnitPos.y-static_cast<int>(gs.cameraDir.y),
                lookAtUnitPos.z-static_cast<int>(gs.cameraDir.z)};
        Chunk::setUnitAtGlobalPos(Unit::ORE,chunks,lookAtUnitFrontPos.x,lookAtUnitFrontPos.y,lookAtUnitFrontPos.z);
    }
}

ListUtilVecInt PlayerWorldInteraction::rayCast(std::vector<Chunk> &chunks, glm::vec3 &rayDir, glm::vec3 &rayStartPos, int maxChunkDistance) {
    bool isAir = true;
    glm::vec3 rayPos = glm::vec{rayStartPos};
    while (isAir && length(rayPos-rayStartPos) < static_cast<float>(maxChunkDistance)*Chunk::LENGTH) {
        rayPos += rayDir;
        if (Chunk::getUnitAtGlobalPos(chunks,rayPos.x,rayPos.y,rayPos.z) != Unit::NONE) isAir = false;
    }
    return {static_cast<int>(rayPos.x),static_cast<int>(rayPos.y),static_cast<int>(rayPos.z)};
}
