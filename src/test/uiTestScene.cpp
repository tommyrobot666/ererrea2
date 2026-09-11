#include <test/uiTestScene.h>

#include <core/gameState.h>

uiTestScene::uiTestScene() {
}

void uiTestScene::load() {
    Renderer::defaultShader();

    int lengthOfUiAtlas = 256;
    uiSprites = new Sprites(Renderer::loadPngTextureNearest("uiAtlas.png"),lengthOfUiAtlas);
    uiSprites->addSprite(0,0,20,8);

    testUi = new uiObject(*uiSprites,glm::vec2(0,0),glm::vec2(gs.gameWindowWidth,160));
}

void uiTestScene::simulate() {
    testUi->startUiUpdate();
    testUi->uiUpdate();
}

void uiTestScene::render() {
    Renderer::clear(0,0,1,1);

    testUi->renderThenChildren(gs.orthoScreenView);
}
