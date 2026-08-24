#pragma once
#include <core/scene.h>
#include <glm/glm.hpp>
#include <uicore/uiObject.h>
#include <core/renderer.h>

class uiTestScene : public Scene{
    Sprites* uiSprites = nullptr;
    uiObject* testUi = nullptr;
public:
    uiTestScene();

    void load() override;
    void simulate() override;
    void render() override;
};
