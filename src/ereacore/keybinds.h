#pragma once

#include <GLFW/glfw3.h>
#include <core/inputManager.h>

class Keybinds {
    public:
    Keybind sprint = GLFW_KEY_LEFT_SHIFT;
    Keybind posZ = GLFW_KEY_W;
    Keybind negZ = GLFW_KEY_S;
    Keybind posX = GLFW_KEY_D;
    Keybind negX = GLFW_KEY_A;
    Keybind posY = GLFW_KEY_E;
    Keybind negY = GLFW_KEY_Q;
    Keybind mine = GLFW_KEY_SPACE;
    Keybind place = GLFW_KEY_LEFT_ALT;
    Keybind up = GLFW_KEY_UP;
    Keybind down = GLFW_KEY_DOWN;
    Keybind left = GLFW_KEY_LEFT;
    Keybind right = GLFW_KEY_RIGHT;
};
