#pragma once
#include <core/scene.h>
#include <uicore/rootUiObject.h>

class uiTestScene : public Scene{
    Sprites* uiSprites = nullptr;
    rootUiObject* testUi = nullptr;
public:
    uiTestScene();

    void load() override;
    void simulate() override;
    void render() override;
};
