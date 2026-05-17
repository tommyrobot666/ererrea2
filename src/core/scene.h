#pragma once

class Scene {
public:
    virtual ~Scene() = default;

    virtual void load() {}

    virtual void simulate() {}

    virtual void render() {}
};
