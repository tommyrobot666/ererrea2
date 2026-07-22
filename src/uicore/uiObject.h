#pragma once

#include <glm/glm.hpp>
#include <core/renderer.h>

namespace uiObjectConstants {
    enum Anchor {
        TOP_LEFT
    };

    // FUCK YOU c++
    // THIS IS JUST WHAT I WANTED TO FUCKING DO WITH "static"
    // DO YOU EVEN FUCKING KNOW WHAT "static" MEANS!?!?!??!?!?!?!?!??
    extern VertexObject* basicQuad;
    extern unsigned int defaultTexture;
}

class uiObject {
    glm::vec2 pos;
    glm::vec2 size;

    bool mouseOver = false;
    bool mouseDown = false;
    bool justClicked = false;
public:
    virtual ~uiObject() = default;

    // use anchor,minCorner,maxCorner to calculate pos,size
    uiObjectConstants::Anchor anchor;
    glm::vec2 minCorner;
    glm::vec2 maxCorner;

    static void initUiSystem();
    static void closeUiSystem();
    void recalculatePosAndSize();
    void startUiUpdate();
    virtual void render();
    virtual void uiUpdate() {};
};
