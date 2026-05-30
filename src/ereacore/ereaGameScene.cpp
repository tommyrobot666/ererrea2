#include <ereacore/ereaGameScene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <core/gameState.h>

void ereaGameScene::load() {
    // chunkGenerator.debugTex = dirtTexture;
    // chunkGenerator.debugtex();
    chunkGenerator.seed = 123;

    blockRenderer.load();

    gs.cameraPos = glm::vec3(0,9,0);
}

void ereaGameScene::simulate() {
    playerWorldInteraction.moveCamera();
    chunkGenerator.generateNearbyChunks(chunks);
    playerWorldInteraction.interactWithUnits(chunks);
}

void ereaGameScene::render() {
    Renderer::clear(1.0f,1.0f,1.0f,1.0f);

    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)GAME_WINDOW_WIDTH/(float)GAME_WINDOW_HEIGHT, 0.1f, 100.0f);

    blockRenderer.render(chunks, proj);
}

void ereaGameScene::close() {
    // delete(chunks);
    blockRenderer.close();
}

