#pragma once

class scene {
public:
    virtual ~scene() = default;

    virtual void load() {}

    virtual void simulate() {}

    virtual void render() {}
};
