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
    // only works when ray is positive numbers because posToIdx isn't made for negiative nombers

    int maxChunkDistance = 2;
    if (glfwGetKey(gs.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        auto rayDir = glm::vec{gs.cameraDir};
        auto rayPos = glm::vec{gs.cameraPos};

        bool isAir = true;
        while (isAir && length(rayPos-gs.cameraPos) < maxChunkDistance*Chunk::LENGTH) {
            rayPos += rayDir;
            Chunk* chunk = Chunk::findChunkOrNone(chunks,
                rayPos.x/Chunk::LENGTH,rayPos.y/Chunk::LENGTH,rayPos.z/Chunk::LENGTH);
            if (chunk == nullptr) continue;
            if (chunk->units[posToIdx(
                static_cast<int>(abs(rayPos.x))%Chunk::LENGTH,
                static_cast<int>(abs(rayPos.y))%Chunk::LENGTH,
                static_cast<int>(abs(rayPos.z))%Chunk::LENGTH,
                Chunk::LENGTH)] != Unit::NONE) isAir = false;
        }

        Chunk* chunk = Chunk::findChunkOrNone(chunks,
                rayPos.x/Chunk::LENGTH,rayPos.y/Chunk::LENGTH,rayPos.z/Chunk::LENGTH);
        if (chunk != nullptr) {
            chunk->units[posToIdx(
                static_cast<int>(abs(rayPos.x))%Chunk::LENGTH,
                static_cast<int>(abs(rayPos.y))%Chunk::LENGTH,
                static_cast<int>(abs(rayPos.z))%Chunk::LENGTH,
                Chunk::LENGTH)] = Unit::NONE;
        }
    }

    auto rayDir = glm::vec{gs.cameraDir};
    auto rayPos = glm::vec{gs.cameraPos};

    bool isAir = true;
    while (isAir && length(rayPos-gs.cameraPos) < maxChunkDistance*Chunk::LENGTH) {
        rayPos += rayDir;
        Chunk* chunk = Chunk::findChunkOrNone(chunks,
            rayPos.x/Chunk::LENGTH,rayPos.y/Chunk::LENGTH,rayPos.z/Chunk::LENGTH);
        if (chunk == nullptr) continue;
        if (chunk->units[posToIdx(
            static_cast<int>(abs(rayPos.x))%Chunk::LENGTH,
            static_cast<int>(abs(rayPos.y))%Chunk::LENGTH,
            static_cast<int>(abs(rayPos.z))%Chunk::LENGTH,
            Chunk::LENGTH)] != Unit::NONE) isAir = false;
    }

    Chunk* chunk = Chunk::findChunkOrNone(chunks,
            rayPos.x/Chunk::LENGTH,rayPos.y/Chunk::LENGTH,rayPos.z/Chunk::LENGTH);
    if (chunk != nullptr) {
        chunk->units[posToIdx(
            static_cast<int>(abs(rayPos.x))%Chunk::LENGTH,
            static_cast<int>(abs(rayPos.y))%Chunk::LENGTH,
            static_cast<int>(abs(rayPos.z))%Chunk::LENGTH,
            Chunk::LENGTH)] = Unit::STONE;
    }
}