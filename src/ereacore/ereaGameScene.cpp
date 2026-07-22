#include <ereacore/ereaGameScene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <core/gameState.h>

void ereaGameScene::load() {
    chunkGenerator.seed = 123;

    unitRenderer.load();
    // Renderer::freeTexture(unitRenderer.atlasTexture);
    // chunkGenerator.debugtex(unitRenderer.atlasTexture);
    gs.cameraPos = glm::vec3(0,9,0);

    testUi.minCorner = glm::vec2(0,0);
    testUi.maxCorner = glm::vec2(8,20);
    testUi.recalculatePosAndSize();
}

void ereaGameScene::simulate() {
    testUi.startUiUpdate();
    testUi.uiUpdate();
    playerWorldInteraction.moveCamera();
    chunkGenerator.generateNearbyChunks(chunks);
    playerWorldInteraction.interactWithUnits(chunks);
}

void ereaGameScene::render() {
    Renderer::clear(1.0f,1.0f,1.0f,1.0f);

    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)gs.gameWindowWidth/(float)gs.gameWindowHeight, 0.1f, 100.0f);

    unitRenderer.render(chunks, proj);

    // gs.r().defaultShader();
    testUi.renderThenChildren();
}

void ereaGameScene::close() {}

