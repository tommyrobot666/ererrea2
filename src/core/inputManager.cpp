#include <core/inputManager.h>
#include <core/gameState.h>
#include <GLFW/glfw3.h>

Keybind::Keybind(const int keyScancode) {
    assert(keyScancode > GLFW_KEY_SPACE && keyScancode < GLFW_KEY_LAST);

    keys.insert(glfwGetKeyScancode(keyScancode));
}

Keybind::Keybind(const int *keyScancodes, const int len) {
    assert(keyScancodes != nullptr);

    for (int i = 0; i < len; ++i) {
        this->keys.insert(glfwGetKeyScancode(keyScancodes[i]));
    }
}

bool InputManager::isPressed(const Keybind &keybind) const {
    return std::any_of(keybind.keys.begin(),keybind.keys.end(),[this](const int key) {
        return currentKeys.count(key) != 0;
    });
}

bool InputManager::isReleased(const Keybind &keybind) const {
    return !isPressed(keybind);
}

bool InputManager::isJustPressed(Keybind &keybind) const {
    if (isReleased(keybind)) return false;
    return std::none_of(keybind.keys.begin(),keybind.keys.end(),[this](const int key) {
        return lastKeys.count(key) != 0;
    });
}

bool InputManager::isJustReleased(Keybind &keybind) const {
    if (isPressed(keybind)) return false;
    return std::any_of(keybind.keys.begin(),keybind.keys.end(),[this](const int key) {
        return lastKeys.count(key) != 0;
    });
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

int InputManager::getLastScancode() const {
    return lastScancode;
}

bool InputManager::isKeyPressed(const int key) const {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool InputManager::isKeyReleased(const int key) const {
    return glfwGetKey(window, key) == GLFW_RELEASE;
}

bool InputManager::excape() const {
    return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}
