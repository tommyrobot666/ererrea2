#include <ereacore/PlayerWorldInteraction.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <3dListUtil.h>
#include <core/gameState.h>
#include <ereacore/chunk.h>
#include <ereacore/keybinds.h>

void PlayerWorldInteraction::moveCamera() {
    float sprint = 1.0f;
    if (gs.input->isPressed(keybinds.sprint))
        sprint = 68.0f;

    // if (gs.input->isPressed(keybinds.left))
    if (glfwGetKey(gs.window, GLFW_KEY_LEFT) == GLFW_PRESS)
        gs.yaw -= 1.3f*gs.deltaTime*sprint;
    // if (gs.input->isPressed(keybinds.right))
    if (glfwGetKey(gs.window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        gs.yaw += 1.3f*gs.deltaTime*sprint;
    // if (gs.input->isPressed(keybinds.down))
    if (glfwGetKey(gs.window, GLFW_KEY_DOWN) == GLFW_PRESS)
        gs.pitch -= 1.3f*gs.deltaTime*sprint;
    // if (gs.input->isPressed(keybinds.up))
    if (glfwGetKey(gs.window, GLFW_KEY_UP) == GLFW_PRESS)
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
        glfwSetWindowShouldClose(gs.window, true);
}

void PlayerWorldInteraction::interactWithUnits(std::vector<Chunk>& chunks) {
    auto raycastHit = rayCastNullable(chunks,gs.cameraDir,gs.cameraPos,2);
    if (raycastHit == nullptr) return;
    auto lookAtUnitPos = ListUtilVecInt(*raycastHit);
    delete raycastHit;

    if (gs.input->isJustPressed(keybinds.mine)) {
        Chunk::setUnitAtGlobalPos(Unit::NONE,chunks,lookAtUnitPos.x,lookAtUnitPos.y,lookAtUnitPos.z);
    } else {
        if (Chunk::getUnitAtGlobalPos(chunks,lookAtUnitPos.x,lookAtUnitPos.y,lookAtUnitPos.z) != Unit::ORE)
        Chunk::setUnitAtGlobalPos(Unit::STONE,chunks,lookAtUnitPos.x,lookAtUnitPos.y,lookAtUnitPos.z);
    }

    if (gs.input->isJustPressed(keybinds.place)) {
        auto lookAtUnitFrontPos = ListUtilVecInt{
                lookAtUnitPos.x-static_cast<int>(gs.cameraDir.x),
                lookAtUnitPos.y-static_cast<int>(gs.cameraDir.y),
                lookAtUnitPos.z-static_cast<int>(gs.cameraDir.z)};
        Chunk::setUnitAtGlobalPos(Unit::ORE,chunks,lookAtUnitFrontPos.x,lookAtUnitFrontPos.y,lookAtUnitFrontPos.z);
    }

    if ((lookAtUnitPos.x!=lastChangex&&lastChangey!=lookAtUnitPos.y&&lookAtUnitPos.z!=lastChangez)||true) {
        lastChangex = lookAtUnitPos.x;
        lastChangey = lookAtUnitPos.y;
        lastChangez = lookAtUnitPos.z;
        auto chunk = Chunk::findChunkOrNone(chunks,lookAtUnitPos.x/Chunk::LENGTH,lookAtUnitPos.y/Chunk::LENGTH,lookAtUnitPos.z/Chunk::LENGTH);
        if (chunk) {
            chunk->queueMeshRegen = true;
            std::cout << "qr\n";
        }
        std::cout << "c\n";
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

// to interpret going till max distance as a "fail"
ListUtilVecInt* PlayerWorldInteraction::rayCastNullable(std::vector<Chunk> &chunks, glm::vec3 &rayDir,
    glm::vec3 &rayStartPos, int maxChunkDistance) {
    auto hit = rayCast(chunks,rayDir,rayStartPos,maxChunkDistance);
    // std::cout << hit.x << "," << hit.y << "," << hit.z << "\n";
    // the -1.8 at the end is a margin to prevent what should be a fail being interpreted as a success
    // 1.8 is a bit more than the distance between the corners of a unit square
    if (length(glm::vec3(hit.x,hit.y,hit.z)-rayStartPos) >= static_cast<float>(maxChunkDistance)*Chunk::LENGTH-1.8) {
        // std::cout << "out\n";
        return nullptr;
    } else {
        return new ListUtilVecInt(hit);
    }
}
