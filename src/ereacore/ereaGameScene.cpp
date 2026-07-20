#include <ereacore/ereaGameScene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <core/gameState.h>

void ereaGameScene::load() {
    chunkGenerator.seed = 123;

    unitRenderer.load();
    // chunkGenerator.debugtex(unitRenderer.dirtTexture);
    unitRenderer.queueMeshRegen = &queueMeshRegen;
    playerWorldInteraction.queueMeshRegen = &queueMeshRegen;

    gs.cameraPos = glm::vec3(0,9,0);
}

void ereaGameScene::simulate() {
    playerWorldInteraction.moveCamera();
    chunkGenerator.generateNearbyChunks(chunks);
    playerWorldInteraction.interactWithUnits(chunks);
}

void ereaGameScene::render() {
    Renderer::clear(1.0f,1.0f,1.0f,1.0f);

    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)gs.gameWindowWidth/(float)gs.gameWindowHeight, 0.1f, 100.0f);

    unitRenderer.render(chunks, proj);
}

void ereaGameScene::close() {}

