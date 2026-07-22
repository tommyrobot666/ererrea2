#pragma once

#include <glm/glm.hpp>
#include <core/renderer.h>

class uiObject {
    glm::vec2 pos;
    glm::vec2 size;

    bool mouseOver = false;
    bool mouseDown = false;
    bool justClicked = false;
public:
    enum Anchor {
        TOP_LEFT
    };

    virtual ~uiObject() = default;

    static VertexObject* basicQuad;
    static unsigned int defaultTexture;

    // use anchor,minCorner,maxCorner to calculate pos,size
    Anchor anchor;
    glm::vec2 minCorner;
    glm::vec2 maxCorner;

    static void initUiSystem();
    static void closeUiSystem();
    void recalculatePosAndSize();
    void startUiUpdate();
    virtual void render();
    virtual void uiUpdate() {};
};
