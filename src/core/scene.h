#pragma once

class Scene {
    bool loaded = false;
    bool closed = false;

    virtual void close() {}

    virtual void load() {}

public:
    virtual ~Scene() = default;

    // It's the opposite of tryLoad?
    void tryClose() {
        if (!closed) {
            close();
            closed = true;
        }
    }

    // If you create a loading bar scene, you can now load the next scene before switching
    void tryLoad() {
        if (!loaded) {
            load();
            loaded = true;
        }
    }

    virtual void simulate() {}

    virtual void render() {}
};
