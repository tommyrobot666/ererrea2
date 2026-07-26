#include <ereacore/ereaGameScene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <core/gameState.h>
#include <core/vertexObjectGenerators.h>
#include <uicore/uiObject.h>

void ereaGameScene::load() {
    chunkGenerator->seed = 123;

    unitRenderer.load();
    // Renderer::freeTexture(unitRenderer.atlasTexture);
    // chunkGenerator.debugtex(unitRenderer.atlasTexture);
    gs.cameraPos = glm::vec3(0,9,0);

    float* vertices = vertexObjectGenerators::quad::vertices();
    unsigned int* indices = reinterpret_cast<unsigned int*>(vertexObjectGenerators::quad::indices());
    uiObjectConstants::basicQuad = Renderer::createVertexObject(vertices,indices,32*sizeof(float),6*sizeof(int));

    testUi.minCorner = glm::vec2(0,0);
    testUi.maxCorner = glm::vec2(40,160);
    testUi.recalculatePosAndSize();

    makeNewChunkGenerationThread();
}

void ereaGameScene::simulate() {
    testUi.startUiUpdate();
    testUi.uiUpdate();
    playerWorldInteraction.moveCamera();
    playerWorldInteraction.interactWithUnits(chunks);
}

void ereaGameScene::makeNewChunkGenerationThread() {
    chunkGenerationThread = std::thread([](void* vp,std::vector<Chunk> chunks) {
        terrainGenerator* chunkGenerator = (terrainGenerator*)vp;
        while (true) {
            chunkGenerator->generateNearbyChunks(chunks);
        }
    },(void*)chunkGenerator,chunks);
    chunkGenerationThread.detach();
}

void ereaGameScene::render() {
    Renderer::clear(1.0f,1.0f,1.0f,1.0f);

    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)gs.gameWindowWidth/(float)gs.gameWindowHeight, 0.1f, 100.0f);
    glm::mat4 orthProj = glm::ortho(0.f,(float)gs.gameWindowWidth,0.f,(float)gs.gameWindowHeight);//,-.1f,.1f);

    Renderer::enableDepthTest();
    unitRenderer.render(chunks, proj);

    drawCubeAtLookedAtUnit(proj);

    Renderer::disableDepthTest();
    testUi.renderThenChildren(orthProj);
}

void ereaGameScene::drawCubeAtLookedAtUnit(glm::mat4& proj) {
    auto cubeModel = unitRenderer.cubeModel;
    cubeModel->currentBind();
    glm::vec3 lastLookedBlock = glm::vec3(playerWorldInteraction.lastLookedBlock.x,
                                          playerWorldInteraction.lastLookedBlock.y,playerWorldInteraction.lastLookedBlock.z);
    glm::mat4 trans = glm::translate(glm::mat4(1.0), lastLookedBlock);
    trans = glm::scale(trans, glm::vec3(1.1));
    trans = glm::translate(trans, glm::vec3(-0.05));
    trans = proj*gs.view*trans;
    gs.r().setShaderTransform(&trans);
    cubeModel->draw();
}

void ereaGameScene::close() {
    chunkGenerationThread.join();
}

