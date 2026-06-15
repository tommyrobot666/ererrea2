#pragma once

#include <GLFW/glfw3.h>
#include <core/inputManager.h>

class Keybinds {
    public:
    Keybind sprint = Keybind(GLFW_KEY_LEFT_SHIFT);
    Keybind posZ = Keybind(GLFW_KEY_W);
    Keybind negZ = Keybind(GLFW_KEY_S);
    Keybind posX = Keybind(GLFW_KEY_D);
    Keybind negX = Keybind(GLFW_KEY_A);
    Keybind posY = Keybind(GLFW_KEY_E);
    Keybind negY = Keybind(GLFW_KEY_Q);
    Keybind mine = Keybind(GLFW_KEY_SPACE);
    Keybind place = Keybind(GLFW_KEY_LEFT_ALT);
    // Keybind up = Keybind(GLFW_KEY_UP);
    // Keybind down = Keybind(GLFW_KEY_DOWN);
    // Keybind left = Keybind(GLFW_KEY_LEFT);
    // Keybind right = Keybind(GLFW_KEY_RIGHT);
};
