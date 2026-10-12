#include <test/uiTestScene.h>

#include <core/gameState.h>

uiTestScene::uiTestScene() {
}

void uiTestScene::load() {
    Renderer::defaultShader();

    int lengthOfUiAtlas = 256;
    uiSprites = new Sprites(Renderer::loadPngTextureNearest("uiAtlas.png"),lengthOfUiAtlas);
    uiSprites->addSprite(0,0,20,8);

    testUi = new rootUiObject(*uiSprites,glm::vec2(200,160),glm::vec2(20,30),OrthProjManager::Anchor::TOP_LEFT);
}

void uiTestScene::simulate() {
    testUi->updateAll();
}

void uiTestScene::render() {
    Renderer::clear(0,0,1,1);

    testUi->renderAll();
}
