#include <uicore/uiObject.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <core/renderer.h>
#include <core/gameState.h>

unsigned int uiObjectConstants::defaultTexture = 0;
VertexObject* uiObjectConstants::basicQuad = 0;

void uiObject::initUiSystem() {
    uiObjectConstants::defaultTexture = Renderer::loadPngTexture("smile.png");
}

void uiObject::closeUiSystem() {
    Renderer::freeTexture(uiObjectConstants::defaultTexture);
    // free quad?
}

void uiObject::recalculatePosAndSize() {
    switch (anchor) {
        case uiObjectConstants::Anchor::TOP_LEFT: {
            pos = minCorner;
            size = maxCorner-minCorner;
        }
        default: return;
    }
}

void uiObject::startUiUpdate() {
    if (justClicked) {
        justClicked = false;
    }

    double mx,my;
    glfwGetCursorPos(gs.window,&mx,&my);
    if (mx>=minCorner.x&&mx<=maxCorner.x&&my>=minCorner.y&&my<=maxCorner.y) {
        mouseOver = true;
    } else {
        mouseOver = false;
        mouseDown = false;
    }

    if (mouseOver && glfwGetMouseButton(gs.window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
        if (!mouseDown) {
            justClicked = true;
            mouseDown = true;
        }
    } else {
        mouseDown = false;
    }

    for (auto &child : children) {
        child.startUiUpdate();
    }
    for (auto &child : children) {
        child.uiUpdate();
    }
}

void uiObject::renderThenChildren(glm::mat4& orthProj) {
    render(orthProj);
    for (auto &child : children) {
        child.renderThenChildren(orthProj);
    }
}

void uiObject::render(glm::mat4& orthProj) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans,glm::vec3(pos.x/gs.gameWindowWidth-1,pos.y/gs.gameWindowHeight+1,0));
    trans = glm::translate(trans, glm::vec3(size.x/2/gs.gameWindowWidth,-size.y/2/gs.gameWindowHeight,0));
    trans = glm::scale(trans, glm::vec3(size.x/gs.gameWindowWidth,size.y/gs.gameWindowHeight,1));
    gs.r().setShaderTransform(&trans);
    Renderer::currentTexture(uiObjectConstants::defaultTexture);
    uiObjectConstants::basicQuad->currentBind();
    uiObjectConstants::basicQuad->draw();
}
