#pragma once

#include <set>
#include <GLFW/glfw3.h>

class Keybind {
    public:
    std::set<int> keys;
    Keybind() {}

    Keybind(int key);
    Keybind(int* keys, int len);
};

class InputManager {
    GLFWwindow *window;
    std::set<int> lastKeys;
    std::set<int> currentKeys;
    int lastScancode;
    public:
    InputManager(GLFWwindow *window) : window(window) {};
    bool isPressed(Keybind &keybind);
    bool isReleased(Keybind &keybind);
    bool isJustPressed(Keybind &keybind);
    bool isJustReleased(Keybind &keybind);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void nextFrame();
    int getLastScancode();
    bool isKeyPressed(int key);
    bool isKeyReleased(int key);
    bool excape();
};
