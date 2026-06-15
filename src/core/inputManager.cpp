#include <core/inputManager.h>
#include <core/gameState.h>
#include <GLFW/glfw3.h>

Keybind::Keybind(int *keys, int len) {
    for (int i = 0; i < len; ++i) {
        this->keys.insert(glfwGetKeyScancode(keys[i]));
    }
}

bool InputManager::isPressed(Keybind &keybind) {
    for (int key : keybind.keys) {
        if (currentKeys.count(key) != 0) return true;
    }
    return false;
}

bool InputManager::isReleased(Keybind &keybind) {
    for (int key : keybind.keys) {
        if (currentKeys.count(key) == 0) return true;
    }
    return false;
}

bool InputManager::isJustPressed(Keybind &keybind) {
    if (!isPressed(keybind)) return false;
    for (int key : keybind.keys) {
        if (lastKeys.count(key) == 0) return true;
    }
    return false;
}

bool InputManager::isJustReleased(Keybind &keybind) {
    if (!isReleased(keybind)) return false;
    for (int key : keybind.keys) {
        if (lastKeys.count(key) != 0) return true;
    }
    return false;
}

void InputManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    gs.input->lastScancode = scancode;
    if (action == GLFW_PRESS) {
        if (gs.input->currentKeys.count(scancode) == 0) {
            gs.input->currentKeys.insert(scancode);
        }
    } else if (action == GLFW_RELEASE) {
        if (gs.input->currentKeys.count(scancode) != 0) {
            gs.input->currentKeys.erase(gs.input->currentKeys.find(scancode));
        }
    }
}

void InputManager::nextFrame() {
    lastKeys.clear();
    for (int key : currentKeys) {
        lastKeys.insert(key);
    }
}

int InputManager::getLastScancode() {
    return lastScancode;
}

bool InputManager::isKeyPressed(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool InputManager::isKeyReleased(int key) {
    return glfwGetKey(window, key) == GLFW_RELEASE;
}
