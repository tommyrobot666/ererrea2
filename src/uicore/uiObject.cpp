#include <uicore/uiObject.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <core/gameState.h>

void uiObject::initUiSystem() {}

void uiObject::closeUiSystem() {}

void uiObject::recalculatePosAndSize() {
    assert(minCorner.x <= maxCorner.x && minCorner.y <= maxCorner.y);

    pos = minCorner;
    size = maxCorner-minCorner;

    for (auto &child : children) {
        child.recalculatePosAndSize();
    }
}

void uiObject::startUiUpdate() {
    if (justClicked) {
        justClicked = false;
    }

    double mx,my;
    glfwGetCursorPos(gs.mainWindow,&mx,&my);
    if (mx>=minCorner.x&&mx<=maxCorner.x&&my>=minCorner.y&&my<=maxCorner.y) {
        mouseOver = true;
    } else {
        mouseOver = false;
        mouseDown = false;
    }

    if (mouseOver && glfwGetMouseButton(gs.mainWindow,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
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
    sprites.draw(0,pos.x,pos.y,size.x,size.y,orthProj);
}
