#pragma once
#include <uicore/uiObject.h>

class rootUiObject : public uiObject {
    public:
    OrthProjManager::Anchor anchor = OrthProjManager::Anchor::TOP_LEFT;

    rootUiObject(Sprites &sprites, const glm::vec2 &maxCorner, const glm::vec2 &minCorner,
        OrthProjManager::Anchor anchor)
        : uiObject(sprites, maxCorner, minCorner),
          anchor(anchor) {
    }

    void renderAll();
    void updateAll();
};

