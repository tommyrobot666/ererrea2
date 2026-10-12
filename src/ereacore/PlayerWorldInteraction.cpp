#include <ereacore/PlayerWorldInteraction.h>

#include <iostream>
#include <glm/vec3.hpp>
#include <core/gameState.h>
#include <ereacore/chunk.h>
#include <ereacore/keybinds.h>

void PlayerWorldInteraction::moveCamera() {
    float sprint = 1.0f;
    if (gs.input->isPressed(keybinds.sprint))
        sprint = 68.0f;

    if (gs.input->isPressed(keybinds.left))
        gs.yaw -= 1.3f*gs.deltaTime*sprint;
    if (gs.input->isPressed(keybinds.right))
        gs.yaw += 1.3f*gs.deltaTime*sprint;
    if (gs.input->isPressed(keybinds.down))
        gs.pitch -= 1.3f*gs.deltaTime*sprint;
    if (gs.input->isPressed(keybinds.up))
        gs.pitch += 1.3f*gs.deltaTime*sprint;

    if (gs.input->isPressed(keybinds.posZ))
        gs.cameraPos.z += 2.7f*gs.deltaTime*sprint;
    if (gs.input->isPressed(keybinds.negZ))
        gs.cameraPos.z -= 2.7f*gs.deltaTime*sprint;
    if (gs.input->isPressed(keybinds.posX))
        gs.cameraPos.x -= 2.7f*gs.deltaTime*sprint;
    if (gs.input->isPressed(keybinds.negX))
        gs.cameraPos.x += 2.7f*gs.deltaTime*sprint;
    if (gs.input->isPressed(keybinds.posY))
        gs.cameraPos.y -= 2.7f*gs.deltaTime*sprint;
    if (gs.input->isPressed(keybinds.negY))
        gs.cameraPos.y += 2.7f*gs.deltaTime*sprint;

    if(gs.input->excape())
        glfwSetWindowShouldClose(gs.mainWindow, true);
}

void PlayerWorldInteraction::interactWithUnits(std::vector<Chunk>& chunks) {
    auto raycastHit = rayCastNullable(chunks,gs.cameraDir,gs.cameraPos,2);
    if (raycastHit == nullptr) return;
    auto lookAtUnitPos = glm::ivec3(*raycastHit);
    delete raycastHit;
    lastLookedBlock = lookAtUnitPos;

    if (gs.input->isJustPressed(keybinds.mine)) {
        Chunk::setUnitAtGlobalPos(Unit::NONE,chunks,lookAtUnitPos.x,lookAtUnitPos.y,lookAtUnitPos.z);
    } else if (gs.input->isJustPressed(keybinds.place)) {
        auto lookAtUnitFrontPos = glm::ivec3{
                lookAtUnitPos.x-static_cast<int>(gs.cameraDir.x),
                lookAtUnitPos.y-static_cast<int>(gs.cameraDir.y),
                lookAtUnitPos.z-static_cast<int>(gs.cameraDir.z)};
        Chunk::setUnitAtGlobalPos(Unit::ORE,chunks,lookAtUnitFrontPos.x,lookAtUnitFrontPos.y,lookAtUnitFrontPos.z);
    }
}

glm::ivec3 PlayerWorldInteraction::rayCast(std::vector<Chunk> &chunks, glm::vec3 &rayDir, glm::vec3 &rayStartPos, int maxChunkDistance) {
    bool isAir = true;
    glm::vec3 rayPos = glm::vec{rayStartPos};
    while (isAir && length(rayPos-rayStartPos) < static_cast<float>(maxChunkDistance)*Chunk::LENGTH) {
        rayPos += rayDir;
        if (Chunk::getUnitAtGlobalPos(chunks,rayPos.x,rayPos.y,rayPos.z) != Unit::NONE) isAir = false;
    }
    return {static_cast<int>(rayPos.x),static_cast<int>(rayPos.y),static_cast<int>(rayPos.z)};
}

glm::ivec3* PlayerWorldInteraction::rayCastNullable(std::vector<Chunk> &chunks, glm::vec3 &rayDir,
    glm::vec3 &rayStartPos, int maxChunkDistance) {
    bool isAir = true;
    glm::vec3 rayPos = glm::vec{rayStartPos};
    while (isAir) {
        rayPos += rayDir;
        if (Chunk::getUnitAtGlobalPos(chunks,rayPos.x,rayPos.y,rayPos.z) != Unit::NONE) isAir = false;

        if (length(rayPos-rayStartPos) >= static_cast<float>(maxChunkDistance)*Chunk::LENGTH) {
            return nullptr;
        }
    }
    return new glm::ivec3{static_cast<int>(rayPos.x),static_cast<int>(rayPos.y),static_cast<int>(rayPos.z)};
}
