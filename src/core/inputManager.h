#pragma once

#include <set>
#include <GLFW/glfw3.h>

class Keybind {
    public:
    std::set<int> keys;
    Keybind() = default;
    // ReSharper disable once CppNonExplicitConvertingConstructor
    Keybind(int keyScancode);
    Keybind(const int* keyScancodes, int len);
};

class InputManager {
    GLFWwindow *window;
    std::set<int> lastKeys;
    std::set<int> currentKeys;
    int lastScancode = 0;
    public:
    explicit InputManager(GLFWwindow *window) : window(window) {};
    bool isPressed(const Keybind &keybind) const; // NOLINT(*-use-nodiscard)
    bool isReleased(const Keybind &keybind) const; // NOLINT(*-use-nodiscard)
    bool isJustPressed(Keybind &keybind) const;
    bool isJustReleased(Keybind &keybind) const;
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void nextFrame();
    int getLastScancode() const; // NOLINT(*-use-nodiscard)
    bool isKeyPressed(int key) const; // NOLINT(*-use-nodiscard)
    bool isKeyReleased(int key) const; // NOLINT(*-use-nodiscard)
    bool excape() const; // NOLINT(*-use-nodiscard)
};
