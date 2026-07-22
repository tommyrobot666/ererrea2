#include <uicore/uiObject.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <core/renderer.h>
#include <core/vertexObjectGenerators.h>
#include <core/gameState.h>

#include "glm/ext/matrix_transform.hpp"

void uiObject::initUiSystem() {
    std::vector<Vertex> vertexes;
    vertexes.push_back(Vertex{glm::vec3(0),glm::vec2(0)});
    vertexes.push_back(Vertex{glm::vec3(1),glm::vec2(1)});
    vertexes.push_back(Vertex{glm::vec3(1,0,0),glm::vec2(1,0)});
    vertexes.push_back(Vertex{glm::vec3(0,1,0),glm::vec2(0,1)});
    std::vector<float> vertexesFloats;
    Vertex::convertToFloats(vertexes,vertexesFloats);
    unsigned int* indices = new unsigned int[]{0,2,3,1,2,3};
    uiObject::basicQuad = Renderer::createVertexObject(vertexesFloats.data(),indices,
        4*vertexObjectGenerators::floatsInVertex,6*sizeof(int));
    free(indices);

    uiObject::defaultTexture = Renderer::loadPngTexture("smile.png");
}

void uiObject::closeUiSystem() {
    Renderer::freeTexture(defaultTexture);
    // free quad?
}

void uiObject::recalculatePosAndSize() {
    switch (anchor) {
        case Anchor::TOP_LEFT: {
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
}

void uiObject::render() {
    glm::mat4 trans = glm::translate(glm::mat4(1.0f),glm::vec3(pos.x,pos.y,0));
    trans = glm::scale(trans, glm::vec3(size.x,size.y,1));
    gs.r().setShaderTransform(&trans);
    Renderer::currentTexture(defaultTexture);
    basicQuad->currentBind();
    basicQuad->draw();
}
