#pragma once

class Scene {
public:
    virtual void close() {}

    virtual void load() {}

    virtual void simulate() {}

    virtual void render() {}
};
