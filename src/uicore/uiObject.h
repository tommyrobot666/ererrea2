#pragma once

#include <glm/glm.hpp>
#include <core/renderer.h>

namespace uiObjectConstants {
    enum Anchor {
        TOP_LEFT
    };
}

class uiObject {
    glm::vec2 pos;
    glm::vec2 size;

    bool mouseOver = false;
    bool mouseDown = false;
    bool justClicked = false;
public:
    uiObject(Sprites &sprites, glm::vec2 maxCorner, glm::vec2 minCorner)
    : sprites(sprites),maxCorner(maxCorner),minCorner(minCorner),pos(minCorner),size(maxCorner-minCorner) {}

    virtual ~uiObject() = default;

    // use anchor,minCorner,maxCorner to calculate pos,size
    uiObjectConstants::Anchor anchor = uiObjectConstants::Anchor::TOP_LEFT;
    glm::vec2 minCorner;
    glm::vec2 maxCorner;
    Sprites& sprites;
    std::vector<uiObject> children;

    static void initUiSystem();
    static void closeUiSystem();
    // don't forget to call after all changes to min/maxCorner done
    void recalculatePosAndSize();
    void startUiUpdate(); // calls update for children, but not for self
    void renderThenChildren(glm::mat4& orthProj);
    virtual void render(glm::mat4& orthProj);
    // also use this to recalculate children min/maxCorner
    virtual void uiUpdate() {};
};
